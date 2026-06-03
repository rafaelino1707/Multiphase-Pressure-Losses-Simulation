#include <iostream>
#include <cmath>
#include <vector>
using namespace std;


struct Pipe_Section
{
    std::string type;
    double initial_diameter;
    double final_diameter;
    double length;
};


class Pipe {
    public:

        void add_geometry(Pipe_Section section);

        void run_solver(double P1, double T_C, double mdot, double dx, double mu, std::string FLUID);

        double diameter(double x);
        
        std::vector<Pipe_Section> sections;

        double total_length() {
            double total = 0.0;

            for (const Pipe_Section& section : sections) {
                total += section.length;
            }

            return total;
        }
        
};


