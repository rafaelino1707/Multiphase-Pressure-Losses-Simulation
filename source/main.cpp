#include "../headers/pipe_geometry.hpp"
#include <iostream>
using namespace std;

int main() {
    Pipe pipe1();

    Pipe_Section section1 = {"D_constant", 10.9, 10.9, 0.2}
    Pipe_Section section2 = {"taper", 10.9, 8, 0.3}
    Pipe_Section section3 = {"D_constant", 8, 8, 0.4}

    pipe1.add_geometry(section1)
    pipe1.add_geometry(section2)
    pipe1.add_geometry(section3)

    cout << "Pipe Diameter = " << pipe1.diameter(0.25) << "m\n";
    return 0;
}