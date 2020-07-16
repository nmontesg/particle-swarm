#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <random>


// random number generation
std::default_random_engine generator;
std::uniform_real_distribution<double> distribution(0, 1);


/**
 * Represents an individual particle in the swarm.
 * All methods are public since they need to be accessed from the Swarm class.
 * @author Nieves Montes Gómez
 */
class Particle {
    private:
        std::vector<double> currentPosition;
        std::vector<double> bestPosition;
        double currentFitness;
        double bestFitness;
        std::vector<double> velocity;

    public:
        Particle(std::vector<std::pair<double,double>> bounds, double (*fitnessFunction)(std::vector<double>));
        void updateVelocity(double inertia, double c1, double c2, std::vector<double> &allTimeBest);
        void updatePosition();
        void updateFitness(double (*fitnessFunction)(std::vector<double>));
        void updateBest();
        void printParticleData();
        double getBestFitness();
        std::vector<double> getBestPosition();    
};


/**
 * Constructor that initialized a particle with random position and velocity within the allowed bounds.
 * @param bounds            Vector of pairs, indicating the allowed ranges in each dimensions of the search space.
 * @param fitnessFunction   Pointer to the function to be optimized. It must take in a vector of doubles and returns a double.
 */
Particle::Particle(std::vector<std::pair<double,double>> bounds, double (*fitnessFunction)(std::vector<double>)) {
    std::vector<std::pair<double,double>>::iterator it;
    for(it = bounds.begin(); it != bounds.end(); it++) {
        // initialize random position
        double number = (it->first) + ((it->second) - (it->first)) * distribution(generator);
        currentPosition.push_back(number);
        bestPosition.push_back(number); // initial position defaults to best so far
        // initialize random velocity
        double position1 = distribution(generator), position2 = distribution(generator);
        velocity.push_back(position2 - position1);
    }
    // initialize current fitness and best fitness
    updateFitness(fitnessFunction);
    bestFitness = currentFitness;
}


/**
 * Update the velocity of the particle according to the properties of the swarm.
 * @param inertia               Inertia weight hyperparameter.
 * @param c1                    Weight of the individual's particle best position so far in the update.
 * @param c2                    Weight of the best position so far across the whole swarm in the update.
 * @param allTimeBestPosition   Best position found so far across the whole swarm.
 */
void Particle::updateVelocity(double inertia, double c1, double c2, std::vector<double> &allTimeBestPosition) {
    int dim = currentPosition.size();
    for (int i = 0; i < dim; i++) {
        velocity[i] = inertia * velocity[i];
        velocity[i] += c1 * distribution(generator) * (bestPosition[i] - currentPosition[i]);
        velocity[i] += c2 * distribution(generator) * (allTimeBestPosition[i] - currentPosition[i]);
    }
}


/**
 * Update the current position by adding the velocity to it
 */
void Particle::updatePosition() {
    std::transform(currentPosition.begin(), currentPosition.end(), velocity.begin(), currentPosition.begin(), std::plus<double>());
}


/**
 * Update the fitness (<i>optimality</i>) of the particle according to the current position.
 * @param fitnessFunction Function to be optimized.
 */
void Particle::updateFitness(double (*fitnessFunction)(std::vector<double>)) {
    currentFitness = fitnessFunction(currentPosition);
}


/**
 * Update the best position and fitness with the current ones if they exceed the old optimal.
 */
void Particle::updateBest() {
    if (currentFitness > bestFitness) {
        bestFitness = currentFitness;
        bestPosition = currentPosition;
    }
}


/**
 * Output particle data to console (for debugging purposes).
 */
void Particle::printParticleData() {
    int dim = currentPosition.size();
    std::cout << "current position: "; 
    for (int i = 0; i < dim; i++) {
        std::cout << currentPosition[i] << "\t";
    }
    std::cout << "\n";

    std::cout << "current fitness: " << currentFitness << "\n";

    std::cout << "best position: "; 
    for (int i = 0; i < dim; i++) {
        std::cout << bestPosition[i] << "\t";
    }
    std::cout << "\n";

    std::cout << "best fitness: " << bestFitness << "\n";

    std::cout << "velocity: "; 
    for (int i = 0; i < dim; i++) {
        std::cout << velocity[i] << "\t";
    }
    std::cout << "\n";
}

/**
 * Retrieve the best optimal the particle has encountered so far.
 * @return Best optimal found by the particle.
 */
double Particle::getBestFitness() {
    return bestFitness;
}

/**
 * Retrieve the best position the particle has encountered so far.
 * @return The vector of the most optimal point found by the particle.
 */
std::vector<double> Particle::getBestPosition() {
    return bestPosition;
}

