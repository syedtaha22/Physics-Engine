#include "Utils/Simulation.hpp"

int main() {
    Utils::Simulation simulation;

    simulation.loadBodiesFromCSV("data/sims/3_body_problem.csv");

    simulation.setSpeed(6);

    while (simulation.isOpen()) {
        simulation.update();
        simulation.draw();
    }
}