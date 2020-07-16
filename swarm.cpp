#include <vector>
#include <algorithm>
#include <tuple>
#include "particle.cpp"


/**
 * Represents a swarm of particles moving accross a bounded space and trying to optimize some given function.
 * @author Nieves Montes Gómez
 */
class Swarm {
    private:
        // private fields
        std::vector<std::pair<double,double>> spaceBounds;
        std::vector<Particle> population;
        double inertia, c1, c2;
        double (*fitnessFunction)(std::vector<double>);
        std::vector<double> bestPositionAllTime;
        double bestFitnessAllTime;

        // private methods
        Particle* findBestCurrentParticle();
        void updateBestAllTime();
        int getSpaceDimensions();
        int getPopulationSize();


    // public methods: constructor & optimization search method
    public:
        Swarm(std::vector<std::pair<double,double>> bounds, int n, double (*fitness)(std::vector<double>), double omega, double d1, double d2);
        std::tuple<std::vector<double>, double, int> optimizationSearch(int maxTotalIter, int maxPartialIter);
};


/**
 * Construct the swarm by randomly initializing the particles, finding and storing the best so far.
 * @param bounds    Vector of pairs, indicating the allowed ranges in each dimensions of the search space.
 * @param popSize   Number of particles in the swarm.
 * @param fitness   Pointer to the function to be optimized. It must take in a vector of doubles and returns a double.
 * @param omega     Inertia weight hyperparameter.
 * @param d1        Weight of the individual's particle best position so far in the update.
 * @param d2        Weight of the best position so far across the whole swarm in the update.
 */
Swarm::Swarm(std::vector<std::pair<double,double>> bounds, int popSize, double (*fitness)(std::vector<double>), double omega, double d1, double d2) {
    spaceBounds = bounds;
    fitnessFunction = fitness;
    inertia = omega;
    c1 = d1;
    c2 = d2;
    // make population of random particles
    for (int i = 0; i < popSize; i++) {
        Particle newParticle (spaceBounds, fitnessFunction);
        population.push_back(newParticle);
    }
    // find and store initial best
    Particle* initialBestParticle = findBestCurrentParticle();
    bestPositionAllTime = initialBestParticle->getBestPosition();
    bestFitnessAllTime = initialBestParticle->getBestFitness();
}

/**
 * Find the particle that is currently in the best position, across the whole swarm.
 * @return A pointer to the partile in the best position, currently.
 */
Particle* Swarm::findBestCurrentParticle() {
    int popSize = getPopulationSize();
    std::vector<double> fitnesses (popSize);
    for (int i = 0; i < popSize; i++) {
        fitnesses[i] = population[i].getBestFitness();
    }
    std::vector<double>::iterator iteratorToMax = std::max_element(fitnesses.begin(), fitnesses.end());
    int bestParticleIndex = std::distance(fitnesses.begin(), iteratorToMax);
    return &(population[bestParticleIndex]);
}

/**
 * Update the best optimal position and optimal function value.
 */
void Swarm::updateBestAllTime() {
    Particle* currentBestParticle = findBestCurrentParticle();
    if (currentBestParticle->getBestFitness() > bestFitnessAllTime) {
        bestPositionAllTime = currentBestParticle->getBestPosition();
        bestFitnessAllTime = currentBestParticle->getBestFitness();
    }
}

/**
 * Get the dimensionality of the search space.
 * @return The number of dimensions of the search space.
 */
int Swarm::getSpaceDimensions() {
    return spaceBounds.size();
}

/**
 * Get the number of particles in the swarm.
 * @return The size of the swarm.
 */
int Swarm::getPopulationSize() {
    return population.size();
}


/**
 * Perform Particle Swarm Optimization.
 * @param maxTotalIter      Maximum iterations before the search is halted.
 * @param maxPartialIter    Maximum iterations <i>without an update of the provisional solution</i> before the search is halted.
 * @return A tuple with the optimal point found, its optimal function value and the number of iterations before halting.
 */
std::tuple<std::vector<double>, double, int> Swarm::optimizationSearch(int maxTotalIter, int maxPartialIter) {
    int totalIter = 1, partialIter = 1;
    std::vector<Particle>::iterator it;
    
    std::vector<double>::iterator aux;
    std::cout << "About to enter search loop" << "\n";
    std::cout << "x* = ";
    for (aux = bestPositionAllTime.begin(); aux != bestPositionAllTime.end(); aux++) {
        std::cout << *aux << "\t";
    }
    std::cout << "\nf* = " << bestFitnessAllTime << "\n\n";

    // optimization loop
    while (totalIter <= maxTotalIter && partialIter <= maxPartialIter) {
        // update velocities of all particles
        for (it = population.begin(); it != population.end(); it++) {
            it->updateVelocity(inertia, c1, c2, bestPositionAllTime);
        }
        // update positions, current fitness and update individual best for all particles
        for (it = population.begin(); it != population.end(); it++) {
            it->updatePosition();
            it->updateFitness(fitnessFunction);
            it->updateBest();
        }
        // update best all time position and fitness across the whole swarm
        double oldBestFitness = bestFitnessAllTime;
        updateBestAllTime();
        if (bestFitnessAllTime > oldBestFitness) {
            partialIter = 1;
        } else {
            partialIter += 1;
        }

        std::cout << "After iteration " << totalIter << ":\n";
        std::cout << "x* = ";
        for (aux = bestPositionAllTime.begin(); aux != bestPositionAllTime.end(); aux++) {
            std::cout << *aux << "\t";
        }
        std::cout << "\nf* = " << bestFitnessAllTime << "\n\n";

        totalIter += 1;
    }
    std::tuple<std::vector<double>, double, int> result (bestPositionAllTime, bestFitnessAllTime, totalIter - 1);
    return result;
    
}