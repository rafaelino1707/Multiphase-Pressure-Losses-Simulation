#include <iostream>
#include "../headers/byphasic_model.hpp"

using namespace std;

double chimsol(double Re_l, double Re_g) {
    return 1.0 + 0.15 * pow(Re_l, 0.687) * pow(Re_g, -0.687);
}

double LM_factor(double chimsol_constant,
    double x_quality,
    double rho_g,
    double rho_l, 
    double mu_l, 
    double mu_g) {

    double Xtt =
    pow((1.0 - x_quality)/x_quality, 0.9) *
    sqrt(rho_g/rho_l) *
    pow(mu_l/mu_g, 0.1);
    
    double lo = 1 + chimsol_constant/Xtt + 1/pow(Xtt, 2);
    return lo;
}

double mu_liquid(double T_K, std::string FLUID) {
    double a0 = -3.79709e-2;
    double a1 = 1.13299e-3;
    double a2 = -1.32530e-5;
    double a3 = 7.99726e-7;
    double a4 = -2.65576e-10;
    double a5 = 4.62884e-13;
    double a6 = -3.31974e-16;

    return a6*pow(T_K, 6) + a5*pow(T_K, 5) + a4*pow(T_K, 4) + a3*pow(T_K, 3) + a2*pow(T_K, 2) + a1*T_K + a0;
}

double mu_vapor(double T_K, std::string FLUID) {
    double a0 = -6.65534-7;
    double a1 = 4.34733e-8;
    double a2 = -1.29535e-10;
    double a3 = -6.45553e-13;
    double a4 = 1.54327e-15;
    double a5 = -2.00025e-18;
    double a6 = 1.11131e-21;

    return a6*pow(T_K, 6) + a5*pow(T_K, 5) + a4*pow(T_K, 4) + a3*pow(T_K, 3) + a2*pow(T_K, 2) + a1*T_K + a0;
}