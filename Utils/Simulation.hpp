#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include <vector>     // For storing vectors of colors and paths
#include <memory>     // For std::shared_ptr
#include <fstream>    // For reading CSV files
#include <sstream>    // For parsing CSV data
#include <iostream>   // For error output

// Include Flat Core for rendering
#include "../Flat/core.hpp"

// Include Physics Core for simulating physics
#include "../Engine/Physics/core.hpp"

// Include Math Core for mathematical operations
#include "../Engine/Math/core.hpp"

#include "Random.hpp" // For generating random colors

namespace Utils {

    // A class to simulate a simple solar system using physics and rendering libraries
    class Simulation {
        Flat::Window window; // Window object for rendering and handling user interactions
        Physics::World world; // Physics world to simulate physical interactions
        std::vector<Flat::Color> colors; // Colors assigned to celestial bodies
        std::string scaleText; // Text to display scale information
        bool started = false; // Flag to toggle simulation start/stop
        bool showGrid = true; // Flag to toggle grid display
        std::vector<Physics::Path> orbits; // Stores the paths (orbits) of celestial bodies
        double speedFactor; // Speed factor for simulation

    private:
        // Function to draw celestial bodies on the window
        void drawBodies() {
            for (int i = 0; i < world.numBodies(); i++) {
                std::shared_ptr<Physics::Body> body = world.getBody(i); // Get the body object

                // Get the position of the body
                const Math::Vector& pos = body->getKinematicProperty(Physics::KinematicProperty::Position);

                orbits[i].insert(pos); // Add the position to the orbit path

                // Draw the body as a tiny circle with a border
                window.drawCircleWithBorder(0.01f, Math::Converter::toVector2f(pos), colors[i], 0.1f, colors[i]);
            }
        }

        // Function to draw orbital paths of celestial bodies
        void DrawOrbits() {
            if (orbits.size() < 2) return;

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

        // Function to display scale and paused state
        void displayScale() {
            float left = window.getCameraLeft();
            float top = window.getCameraTop();
            float padding = abs(top - window.getCameraBottom()) * 0.05f;

            if (!started) {
                window.writeText("Paused", sf::Vector2f(left + padding, top - padding),
                    Flat::Color::White, 1, false, 0.7f); // Display "Paused" when simulation is stopped
            }

            float bottom = window.getCameraBottom();

            window.writeText(scaleText, sf::Vector2f(left + padding, bottom + padding),
                Flat::Color::White, 1, false, 0.7f); // Display scale text
        }

    public:
        // Constructor to initialize the simulation
        Simulation() : window(1280, 768, "Physics Engine from Scratch", Flat::Color::Black, 5.0f, 32) {
            speedFactor = 1e7; // Set the speed factor for the simulation
        }

        // Function to read a CSV file and populate celestial bodies in the simulation
        void loadBodiesFromCSV(const std::string& filename) {
            std::ifstream file(filename);
            if (!file.is_open()) {
                std::cerr << "Error: Unable to open file " << filename << std::endl;
                return;
            }

            std::string line;
            std::getline(file, line); // Skip the header line

            double maxDistance = 0.0; // Maximum distance from the origin

            // Read each line to extract body properties
            while (std::getline(file, line)) {
                std::stringstream ss(line);
                double mass, distanceX, distanceY, velocityX, velocityY;
                char comma;

                // Parse mass, distance, and velocity from the CSV
                ss >> mass >> comma >> distanceX >> comma >> distanceY >> comma >> velocityX >> comma >> velocityY;

                Math::Vector position(distanceX, distanceY); // Position vector
                Math::Vector velocity(velocityX, velocityY); // Velocity vector

                double distance = Math::Operation::Length(position); // Calculate distance from the origin
                double velocityMagnitude = Math::Operation::Length(velocity); // Calculate velocity magnitude

                if (distance > maxDistance) maxDistance = distance; // Update the maximum distance

                // Create a new body with the parsed mass and position
                std::shared_ptr<Physics::Body> body = std::make_shared<Physics::Body>(mass, position);

                body->setKinematicProperty(Physics::KinematicProperty::LinearVelocity, velocity); // Set velocity
                world.addBody(body); // Add the body to the physics world

                colors.push_back(Random::Color()); // Assign a random color to the body
            }

            file.close(); // Close the CSV file

            // Get the order of magnitude of the maximum distance
            double scale = std::pow(10, static_cast<int>(std::log10(maxDistance)) - 1);
            Math::Converter::setScale(scale); // Set the scale for conversion

            // Update the scale text to display the scale information in scientific notation, using sstream
            std::ostringstream oss;
            oss << std::scientific << std::setprecision(0) << scale << " m";
            scaleText = oss.str();

            orbits.resize(world.numBodies()); // Resize the orbits vector to match the number of bodies
        }

        void setSpeed(double factor) {
            Math::Operation::Clamp(factor, 0.0, 15.0); // Clamp the factor between 0 and 10
            speedFactor = std::pow(10, factor); // Set the speed factor for the simulation
        }


        // Function to update the simulation state
        void update() {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();

                // Handle zoom in/out
                if (event.type == sf::Event::MouseWheelScrolled) {
                    if (event.mouseWheelScroll.delta > 0)
                        window.zoom(0.95f); // Zoom in
                    else
                        window.zoom(1.05f); // Zoom out
                }

                // Handle start/stop toggle with Space key
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Space) {
                        started = !started;
                    }

                    // Handle grid toggle with G key
                    if (event.key.code == sf::Keyboard::G) {
                        showGrid = !showGrid;
                    }
                }
            }

            // Advance simulation if started
            if (started) {
                world.step(window.getElapsedTimeSinceLastFrame(Flat::TimeUnit::Seconds) * speedFactor);
            }

            window.update(); // Update the window
        }

        // Function to draw all elements of the simulation
        void draw() {
            window.clear(); // Clear the window

            if (showGrid)
                window.drawGridLines(1.0f, Flat::Color("#333333")); // Draw grid lines

            // Draw celestial bodies
            for (int i = 0; i < world.numBodies(); i++) {
                std::shared_ptr<Physics::Body> body = world.getBody(i);
                const Math::Vector& pos = body->getKinematicProperty(Physics::KinematicProperty::Position);
                orbits[i].insert(pos);
                window.drawCircleWithBorder(0.01f, Math::Converter::toVector2f(pos), colors[i], 0.1f, colors[i]);
            }

            DrawOrbits(); // Draw orbital paths
            displayScale(); // Display scale text

            window.display(); // Display everything on the window
        }

        // Check if the window is open
        bool isOpen() {
            return window.isOpen();
        }
    };
}

#endif // SIMULATION_HPP
