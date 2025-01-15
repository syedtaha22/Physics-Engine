#include "Utils/Simulation.hpp"

int main() {
    Utils::Simulation simulation;

    simulation.init("data/sims/solar-system.csv");

    simulation.setSpeed(7);

    while (simulation.isOpen()) {
        simulation.update();
        simulation.draw();
    }
}