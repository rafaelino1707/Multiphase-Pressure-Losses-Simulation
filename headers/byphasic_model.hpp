#include <iostream>
#include <cmath>

using namespace std;

double chimsol(double Re_l, double Re_g);

double LM_factor(double chimsol_constant,
    double x_quality,
    double rho_g,
    double rho_l, 
    double mu_l, 
    double mu_g);

double mu_liquid(double T_K, std::string FLUID);

double mu_vapor(double T_K, std::string FLUID);