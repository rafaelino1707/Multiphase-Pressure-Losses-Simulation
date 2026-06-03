#include "../headers/pipe_geometry.hpp"
#include <iostream>
#include "CoolProp.h"
using namespace std;

int main() {
    Pipe pipe1;

    Pipe_Section section1 = {"D_constant", 10.9, 10.9, 0.2};
    Pipe_Section section2 = {"taper", 10.9, 8, 0.3};
    Pipe_Section section3 = {"D_constant", 8, 8, 0.4};

    pipe1.add_geometry(section1);
    pipe1.add_geometry(section2);
    pipe1.add_geometry(section3);

    cout << "Pipe Diameter = " << pipe1.diameter(0.48) << "m\n";
    // double psat = CoolProp::PropsSI("P", "T", 273.15, "Q", 0, "NitrousOxide");
    double psat = CoolProp::PropsSI("P", "T", 373.15, "Q", 0, "Water");
    cout << "Psat = " << psat / 1e5 << " bar\n";

    pipe1.run_solver(50, 0, 1.5, 0.001, 2e-4, "NitrousOxide");
    return 0;
}