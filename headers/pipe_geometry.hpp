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

        double diameter(double x);
        double total_length;
        std::vector<Pipe_Section> sections;
        
};


