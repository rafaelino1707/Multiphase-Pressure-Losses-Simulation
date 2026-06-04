#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <CoolProp.h>
#include "../headers/pipe_geometry.hpp"
#include "../headers/byphasic_model.hpp"

using namespace std;

void Pipe::run_solver(double P1_bar, double T_C, double mdot, double dx,
    double mu, std::string FLUID) {

    double P_Pa = P1_bar * 1e5;
    double T_K = T_C + 273.15;

    // Entalpia inicial do líquido comprimido
    double h_total = CoolProp::PropsSI("H", "P", P_Pa, "T", T_K, FLUID);

    std::vector<double> x_vec;

    for (double xi = 0.0; xi <= Pipe::total_length(); xi += dx) {
        x_vec.push_back(xi);
    }

    for (double xi : x_vec) {

        double D_mm = diameter(xi);
        double D = D_mm / 1000.0;
        double A = M_PI * D * D / 4.0;

        double Psat_T = CoolProp::PropsSI("P", "T", T_K, "Q", 0, FLUID);

        bool biphasic = false;

        double rho;
        double V;
        double Re;
        double dpdx;

        if (P_Pa > Psat_T) {
            // =========================
            // SINGLE-PHASE LIQUID
            // =========================

            rho = CoolProp::PropsSI("D", "P", P_Pa, "T", T_K, FLUID);

            V = mdot / (rho * A);
            Re = rho * V * D / mu;

            double f;

            if (Re < 2300.0) {
                f = 64.0 / Re;
            } else {
                f = 0.3164 / pow(Re, 0.25);
            }

            dpdx = f * rho * V * V / (2.0 * D);
        }

        else {
            // =========================
            // BIPHASIC FLOW
            // =========================

            biphasic = true;

            double h_l = CoolProp::PropsSI("H", "P", P_Pa, "Q", 0, FLUID);
            double h_g = CoolProp::PropsSI("H", "P", P_Pa, "Q", 1, FLUID);

            double x_quality = (h_total - h_l) / (h_g - h_l);

            x_quality = std::clamp(x_quality, 1e-6, 1.0 - 1e-6);

            double rho_l = CoolProp::PropsSI("D", "P", P_Pa, "Q", 0, FLUID);
            double rho_g = CoolProp::PropsSI("D", "P", P_Pa, "Q", 1, FLUID);

            double mu_l = mu_liquid(T_K, FLUID);
            double mu_g = mu_vapor(T_K, FLUID);

            // densidade homogénea pela qualidade
            rho = 1.0 / (x_quality/rho_g + (1.0 - x_quality)/rho_l);

            V = mdot / (rho * A);

            // Para Lockhart-Martinelli, calcula-se geralmente dp_lo:
            // perda que existiria se todo o caudal fosse líquido.
            double V_lo = mdot / (rho_l * A);
            double Re_lo = rho_l * V_lo * D / mu_l;

            double f_lo;

            if (Re_lo < 2300.0) {
                f_lo = 64.0 / Re_lo;
            } else {
                f_lo = 0.3164 / pow(Re_lo, 0.25);
            }

            double dpdx_lo = f_lo * rho_l * V_lo * V_lo / (2.0 * D);

            // Reynolds das fases
            double V_l = mdot / (rho_l * A);
            double V_g = mdot / (rho_g * A);

            double Re_l = rho_l * V_l * D / mu_l;
            double Re_g = rho_g * V_g * D / mu_g;

            double C = chimsol(Re_l, Re_g);

            // isto deve devolver phi_lo^2
            double phi_lo_sq = LM_factor(C, x_quality, rho_g, rho_l, mu_l, mu_g);

            dpdx = phi_lo_sq * dpdx_lo;

            Re = Re_lo;

            cout << "Biphasic flow detected at x = " << xi
                 << " m, quality = " << x_quality
                 << ", rho_mix = " << rho
                 << " kg/m3\n";
        }

        cout << "x = " << xi
             << " P = " << P_Pa / 1e5 << " bar"
             << " D = " << D
             << " V = " << V
             << " Re = " << Re
             << " dpdx = " << dpdx
             << endl;

        P_Pa -= dpdx * dx;

        if (P_Pa <= 0 || std::isnan(P_Pa)) {
            cout << "Solver stopped: invalid pressure.\n";
            break;
        }
    }
}