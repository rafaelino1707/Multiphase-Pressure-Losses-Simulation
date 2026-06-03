#include <iostream>
#include "../headers/pipe_geometry.hpp"

using namespace std;

void Pipe::add_geometry(Pipe_Section pipe_section) {
    sections.push_back(pipe_section);
}

double Pipe::diameter(double x) {
    double xn = 0.0;

    for (Pipe_Section section : sections) {
        double xn1 = xn + section.length;

        if (x >= xn && x <= xn1) {
            /* 
            Important, since this gives the x using the section as
            reference and not the entire pipe.
            */
            double local_x = x - xn;

            if (section.type == "D_constant") {
                return section.initial_diameter;
            }

            else if (section.type == "taper") {
                return section.initial_diameter
                 + (section.final_diameter - section.initial_diameter) 
                 * local_x / section.length;
            }
        }

        xn = xn1;
    }

    return -1;
}