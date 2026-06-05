#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <limits>
#include <CoolProp.h>

#include "../headers/pipe_geometry.hpp"
#include "../headers/byphasic_model.hpp"

using namespace std;

struct FlowState {
    double rho;
    double V;
    double h;
    double T;
    double Q;
    double a;
    bool biphasic;
    bool choked;
    bool valid;
};

static double friction_factor(double Re) {
    if (Re <= 0.0 || !std::isfinite(Re)) {
        return std::numeric_limits<double>::quiet_NaN();
    }

    if (Re < 2300.0) {
        return 64.0 / Re;
    }

    // Blasius, Darcy friction factor, smooth pipe
    return 0.3164 / pow(Re, 0.25);
}

static FlowState reconstruct_state(
    double P_Pa,
    double h0,
    double mdot,
    double A,
    const std::string& FLUID
) {
    FlowState s{};
    s.valid = false;

    const int max_iter = 80;
    const double tol = 1e-7;
    const double relax = 0.35;

    double h = h0;

    for (int iter = 0; iter < max_iter; iter++) {
        double rho = CoolProp::PropsSI("D", "P", P_Pa, "H", h, FLUID);

        if (!std::isfinite(rho) || rho <= 0.0) {
            return s;
        }

        double V = mdot / (rho * A);

        if (!std::isfinite(V)) {
            return s;
        }

        double h_candidate = h0 - 0.5 * V * V;

        // Relaxação para evitar saltos violentos perto da saturação
        double h_new = (1.0 - relax) * h + relax * h_candidate;

        double err = std::abs(h_new - h) / std::max(std::abs(h0), 1.0);

        h = h_new;

        if (err < tol) {
            break;
        }
    }

    s.h = h;
    s.rho = CoolProp::PropsSI("D", "P", P_Pa, "H", h, FLUID);

    if (!std::isfinite(s.rho) || s.rho <= 0.0) {
        return s;
    }

    s.V = mdot / (s.rho * A);

    s.T = CoolProp::PropsSI("T", "P", P_Pa, "H", h, FLUID);
    s.Q = CoolProp::PropsSI("Q", "P", P_Pa, "H", h, FLUID);

    // Em CoolProp, Q costuma ser -1 em monofásico.
    s.biphasic = std::isfinite(s.Q) && s.Q >= 0.0 && s.Q <= 1.0;

    // Speed of sound pode falhar em bifásico, por isso protege.
    s.a = std::numeric_limits<double>::quiet_NaN();
    s.choked = false;

    try {
        s.a = CoolProp::PropsSI("A", "P", P_Pa, "H", h, FLUID);

        if (std::isfinite(s.a) && s.a > 0.0) {
            s.choked = s.V >= s.a;
        }
    } catch (...) {
        s.choked = false;
    }

    s.valid = true;
    return s;
}

void Pipe::run_solver(
    double P1_bar,
    double T_C,
    double mdot,
    double dx,
    double mu,
    std::string FLUID
) {
    double P_Pa = P1_bar * 1e5;
    double T_K = T_C + 273.15;

    // Entalpia de estagnação inicial.
    // No inlet, assume-se V ~ 0 para primeira aproximação.
    double h0 = CoolProp::PropsSI("H", "P", P_Pa, "T", T_K, FLUID);

    std::vector<double> x_vec;

    for (double xi = 0.0; xi <= Pipe::total_length(); xi += dx) {
        x_vec.push_back(xi);
    }

    for (double xi : x_vec) {
        double D_mm = diameter(xi);
        double D = D_mm / 1000.0;
        double A = M_PI * D * D / 4.0;

        FlowState state = reconstruct_state(P_Pa, h0, mdot, A, FLUID);

        if (!state.valid) {
            cout << "Solver stopped: invalid thermodynamic state at x = "
                 << xi << " m, P = " << P_Pa / 1e5 << " bar\n";
            break;
        }

        if (state.choked) {
            cout << "Solver stopped: choking detected at x = "
                 << xi << " m, V = " << state.V
                 << " m/s, a = " << state.a << " m/s\n";
            break;
        }

        double dpdx = 0.0;
        double Re_print = 0.0;

        if (!state.biphasic) {
            // =========================
            // SINGLE-PHASE
            // =========================

            double Re = state.rho * state.V * D / mu;
            double f = friction_factor(Re);

            dpdx = f * state.rho * state.V * state.V / (2.0 * D);

            Re_print = Re;
        } else {
            // =========================
            // BIPHASIC HEM + LM
            // =========================

            double x_quality = std::clamp(state.Q, 1e-6, 1.0 - 1e-6);

            double rho_l = CoolProp::PropsSI("D", "P", P_Pa, "Q", 0, FLUID);
            double rho_g = CoolProp::PropsSI("D", "P", P_Pa, "Q", 1, FLUID);

            double mu_l = mu_liquid(state.T, FLUID);
            double mu_g = mu_vapor(state.T, FLUID);

            double G = mdot / A;

            // Liquid-only pressure drop
            double V_lo = G / rho_l;
            double Re_lo = rho_l * V_lo * D / mu_l;
            double f_lo = friction_factor(Re_lo);

            double dpdx_lo = f_lo * rho_l * V_lo * V_lo / (2.0 * D);

            // Gas-only Reynolds for Chisholm constant
            double V_go = G / rho_g;
            double Re_go = rho_g * V_go * D / mu_g;

            double C = chimsol(Re_lo, Re_go);
            double phi_lo_sq = LM_factor(C, x_quality, rho_g, rho_l, mu_l, mu_g);

            dpdx = phi_lo_sq * dpdx_lo;

            Re_print = Re_lo;

            cout << "Biphasic at x = " << xi
                 << " m | Q = " << x_quality
                 << " | rho_HEM = " << state.rho
                 << " kg/m3 | phi_lo^2 = " << phi_lo_sq
                 << "\n";
        }

        if (!std::isfinite(dpdx) || dpdx <= 0.0) {
            cout << "Solver stopped: invalid dpdx at x = "
                 << xi << " m\n";
            break;
        }

        cout << "x = " << xi
             << " P = " << P_Pa / 1e5 << " bar"
             << " D = " << D
             << " rho = " << state.rho
             << " V = " << state.V
             << " Re = " << Re_print
             << " T = " << state.T
             << " Q = " << state.Q
             << " dpdx = " << dpdx
             << endl;

        // Limiter simples para não deixar o Euler dar um salto absurdo
        double dp = dpdx * dx;
        double dp_max = 0.02 * P_Pa; // máximo 2% da pressão local por passo

        if (dp > dp_max) {
            cout << "Pressure step limited: dp = "
                 << dp / 1e5 << " bar -> "
                 << dp_max / 1e5 << " bar\n";

            dp = dp_max;
        }

        P_Pa -= dp;

        if (P_Pa <= 0.0 || std::isnan(P_Pa)) {
            cout << "Solver stopped: invalid pressure.\n";
            break;
        }
    }
}