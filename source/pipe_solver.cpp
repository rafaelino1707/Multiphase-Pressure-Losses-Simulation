#include <iostream>
#include <vector>
#include <cmath>
#include <CoolProp.h>
#include "../headers/pipe_geometry.hpp"

using namespace std;

void Pipe::run_solver(double P1_bar, double T_C, double mdot, double dx,
    double mu, std::string FLUID) {

    double P_Pa = P1_bar * 1e5;   // bar -> Pa, não dividir
    double T_K = T_C + 273.15;

    std::vector<double> x;

    for (double xi = 0.0; xi <= Pipe::total_length(); xi += dx) {
        x.push_back(xi);
    }

    for (double xi : x) {
        double D_mm = diameter(xi);
        double D = D_mm / 1000.0;     // mm -> m

        double rho = CoolProp::PropsSI("D", "P", P_Pa, "T", T_K, FLUID);

        double A = M_PI * D * D / 4.0;
        double V = mdot / (rho * A);

        double Re = rho * V * D / mu;

        double f;

        if (Re < 2300.0) {
            f = 64.0 / Re;
        } else {
            f = 0.3164 / pow(Re, 0.25); // Blasius, tubo liso
        }

        double dpdx = f * rho * V * V / (2.0 * D);


        cout << "x = " << xi
             << " P = " << P_Pa
             << " D = " << D
             << " V = " << V
             << " Re = " << Re
             << " dpdx = " << dpdx
             << endl;

        P_Pa -= dpdx * dx;
    }
}