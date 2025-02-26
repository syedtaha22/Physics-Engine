#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "Utils/Simulation.hpp"
#include "Utils/Random.hpp"

class PlanetSystem : public Utils::Simulation {
    Physics::World world; ///< Physics world to simulate physical interactions
    std::vector<Graphics::Color> colors; ///< Colors assigned to celestial bodies
    std::vector<Physics::Path> orbits; ///< Stores the paths (orbits) of celestial bodies

private:
    void DrawOrbits() {
        if (orbits.size() < 2) return; // Skip if there are fewer than 2 orbits
        for (int i = 0; i < orbits.size(); i++) {
            for (int j = 0; j < orbits[i].getSize() - 1; j++) {
                window.drawLine(
                    Math::Converter::toVector2f(orbits[i].get(j)),
                    Math::Converter::toVector2f(orbits[i].get(j + 1)),
                    colors[i].withAlha(80) // Use a semi-transparent color for the orbit
                );
            }
        }
    }

    void loadBodiesFromCSV(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: Unable to open file " << filename << std::endl;
            return;
        }

        std::string line;
        std::getline(file, line); // Skip the header line

        // Read each line to extract body properties
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            double mass, distanceX, distanceY, velocityX, velocityY;
            char comma;

            // Parse mass, distance, and velocity from the CSV
            ss >> mass >> comma >> distanceX >> comma >> distanceY >> comma >> velocityX >> comma >> velocityY;

            Math::Vector position(distanceX, distanceY); // Position vector
            Math::Vector velocity(velocityX, velocityY); // Velocity vector

            // Create a new body with the parsed mass and position
            std::shared_ptr<Physics::Body> body = std::make_shared<Physics::Body>(mass, position);

            // Set velocity
            body->setKinematicProperty(Physics::KinematicProperty::LinearVelocity, velocity);
            world.addBody(body); // Add the body to the physics world

            colors.push_back(Utils::Random::Color()); // Assign a random color to the body
        }

        orbits.resize(world.numBodies()); // Resize the orbits vector to match the number of bodies
        file.close(); // Close the CSV file
    }

    double get_max_distance() override {
        double maxDistance = 0.0; // Maximum distance from the origin

        for (int i = 0; i < world.numBodies(); i++) {
            // Get the i-th body and calculate its distance from the origin
            std::shared_ptr<Physics::Body> body = world.getBody(i);
            double distance =
                Math::Operation::Length(body->getKinematicProperty(Physics::KinematicProperty::Position));
            if (distance > maxDistance) maxDistance = distance; // Update the maximum distance
        }
        return maxDistance; // Return the maximum distance
    }


public:
    PlanetSystem(std::string filename) : Utils::Simulation("Solar System Simulation") {
        loadBodiesFromCSV(filename); // Load celestial bodies from CSV file
        init();
    }

    void step() override {
        world.step(window.getElapsedTimeSinceLastFrame(Graphics::TimeUnit::Seconds) * speedFactor);
    }

    void draw_bodies() override {
        for (int i = 0; i < world.numBodies(); i++) {
            std::shared_ptr<Physics::Body> body = world.getBody(i); // Get the body object

            // Get the position of the body
            const Math::Vector& pos = body->getKinematicProperty(Physics::KinematicProperty::Position);

            orbits[i].insert(pos); // Add the position to the orbit path

            // Draw the body as a tiny circle with a border
            window.drawCircleWithBorder(0.01f, Math::Converter::toVector2f(pos), colors[i], 0.1f, colors[i]);
        }
        DrawOrbits(); // Draw orbital paths
    }
};


int main() {
    PlanetSystem simulation("data/sims/solar-system.csv");

    simulation.setSpeed(7);

    while (simulation.isOpen()) {
        simulation.update();
        simulation.draw();
    }
}