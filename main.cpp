#include <iostream>
#include <tuple>
#include <vector>
#include "swarm.cpp"


// very dumb example function
double paraboloid(std::vector<double> x) {
    std::vector<double>::iterator it;
    double result = 0;
    for (it = x.begin(); it != x.end(); it++) {
        result -= (*it) * (*it);
    }
    return result;
}


int main(int argc, char *argv[]) {

    // search space 2-dimensional between -1 and 1
    std::pair<double,double> oneDimensionalBounds (-1., 1.);
    std::vector<std::pair<double,double>> bounds (2, oneDimensionalBounds);
    
    Swarm theSwarm (bounds, 500, &paraboloid, 0.75, 0.5, 0.5);
    std::tuple<std::vector<double>, double, int> optimal = theSwarm.optimizationSearch(200, 50);

    return 0;
}