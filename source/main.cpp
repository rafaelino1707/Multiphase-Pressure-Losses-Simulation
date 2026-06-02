#include "../headers/pipe_geometry.hpp"
#include <iostream>
using namespace std;

int main() {
    Pipe pipe1(0.1, 2);

    cout << "Pipe Diameter = " << pipe1.diameter << "m\n";
    return 0;
}