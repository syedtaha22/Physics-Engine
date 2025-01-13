#include <vector>
#include <exception>
#include <memory>
#include <sstream>
#include <iomanip>
#include <unordered_map>
#include <fstream>

// Include Flat Core
#include "Flat/core.hpp"

// Include Physics Core
#include "Engine/Physics/core.hpp"

// Include Math Core
#include "Engine/Math/core.hpp"

#include "Utils/StopWatch.hpp" // Include stopwatch utility
#include "Utils/Random.hpp" // Include random utility

class Simulation {
    Flat::Window window;

    Physics::World world;

    std::vector<Flat::Color> colors;

    std::string scaleText;

    bool started = false;
    bool showGrid = true;

    std::vector<Physics::Path> orbits;

public:
    Simulation() : window(1280, 768, "Physics Engine from Scratch", Flat::Color::Black, 5.0f, 32) {
        // Use ostringstream to convert the double to a string in scientific notation
        std::ostringstream oss;
        oss << std::scientific << std::setprecision(0) << Math::Converter::getScale();

        scaleText = "Scale: " + oss.str();

        // 
        addBodiesFromCSV("Simulations/solar-system.csv");

        // Add colors for the bodies
        /*
            Sun: 255, 204, 102
            Mercury: 169, 169, 169
            Venus: 240, 234, 214
            Earth: 70, 130, 180
            Mars: 210, 105, 30
            Jupiter: 188, 143, 143
            Saturn: 210, 180, 140
            Uranus: 173, 216, 230
            Neptune: 0, 0, 139

        */
        colors.push_back(Flat::Color(255, 204, 102)); // Sun
        colors.push_back(Flat::Color(169, 169, 169)); // Mercury
        colors.push_back(Flat::Color(240, 234, 214)); // Venus
        colors.push_back(Flat::Color(70, 130, 180)); // Earth
        colors.push_back(Flat::Color(210, 105, 30)); // Mars
        colors.push_back(Flat::Color(188, 143, 143)); // Jupiter
        colors.push_back(Flat::Color(210, 180, 140)); // Saturn
        colors.push_back(Flat::Color(173, 216, 230)); // Uranus
        colors.push_back(Flat::Color(0, 0, 139)); // Neptune

        orbits.resize(world.numBodies());
    }


    // Main function to read CSV and populate bodies
    void addBodiesFromCSV(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: Unable to open file " << filename << std::endl;
            return;
        }

        std::string line;
        // Skip the header row
        std::getline(file, line);

        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string bodyName;
            double mass, distance, velocityY;
            char comma;

            // Read the CSV row
            std::getline(ss, bodyName, ',');
            ss >> mass >> comma >> distance >> comma >> velocityY;

            // Create position vector (distance along x-axis)
            Math::Vector position(distance, 0.0);

            // Create velocity vector (tangential velocity along y-axis)
            Math::Vector velocity(0.0, velocityY);

            // Add the body and apply velocity
            std::shared_ptr<Physics::Body> body = Physics::Body::AstronomicalBody(mass, position);
            body->setKinematicProperty("LinearVelocity", velocity);
            world.addBody(body);
        }

        file.close();
    }

    void update() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Check for zoom
            if (event.type == sf::Event::MouseWheelScrolled) {
                if (event.mouseWheelScroll.delta > 0)
                    window.zoom(0.95f); // Zoom in
                else
                    window.zoom(1.05f); // Zoom out
            }

            // Check for Start/Stop
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    started = !started;
                }

                // Check for Grid
                if (event.key.code == sf::Keyboard::G) {
                    showGrid = !showGrid;
                }
            }
        }


        // stopwatch.start();
        if (started) {
            world.step(window.getElapsedTimeSinceLastFrame(Flat::TimeUnit::Seconds) * 1e7);
        }
        // // stopwatch.stop();

        // this->totalStepTime += stopwatch.getElapsedTimeInMilliseconds();
        // this->totalBodyCount += world.numBodies();
        // this->totalSampleCount++;

        // float bottom = window.getCameraBottom();

        // // Loop over the bodies
        // for (int i = 0; i < world.numBodies(); i++) {
        //     FlatPhysics::FlatBody* body = nullptr;

        //     if (!world.getBody(i, body)) {
        //         throw std::runtime_error("Error getting body at index " + std::to_string(i));
        //     }

        //     // If the body is not static
        //     if (!body->isStatic) {
        //         // Get the position
        //         FlatPhysics::FlatAABB aabb = body->getAABB();
        //         //std::cout << "Body " << i << " AABB: " << aabb.min->x << ", " << aabb.min->y << " - " << aabb.max->x << ", " << aabb.max->y << std::endl;
        //         // Check if the body is out of bounds
        //         if (aabb.max->y < bottom) {
        //             world.removeBody(i);
        //             entities.erase(entities.begin() + i);
        //         }
        //     }
        // }



        window.update();
    }


    void draw() {
        window.clear();

        if (showGrid)
            window.drawGridLines(1.0f, Flat::Color("#333333"));

        // Draw the bodies
        for (int i = 0; i < world.numBodies(); i++) {
            std::shared_ptr<Physics::Body> body = world.getBody(i);

            if (!body) {
                throw std::runtime_error("Error getting body at index " + std::to_string(i));
            }

            // Get the position
            Math::Vector pos = body->getKinematicProperty("Position");

            orbits[i].insert(pos);

            window.drawCircleWithBorder(0.01f, Math::Converter::toVector2f(pos), colors[i], 0.1f, colors[i]);

        }

        DrawOrbits();

        displayScale();

        window.display();
    }

    void DrawOrbits() {
        if (orbits.size() < 2) return;

        for (int i = 0; i < orbits.size(); i++) {
            for (int j = 0; j < orbits[i].getSize() - 1; j++) {

                window.drawLine(
                    Math::Converter::toVector2f(orbits[i].get(j)),
                    Math::Converter::toVector2f(orbits[i].get(j + 1)),
                    colors[i].withAlha(80));
            }
        }
    }


    void displayScale() {
        float left = window.getCameraLeft();
        float top = window.getCameraTop();
        float padding = abs(top - window.getCameraBottom()) * 0.05f;

        if (!started) {
            float top = window.getCameraTop();
            window.writeText("Paused", sf::Vector2f(left + padding, top - padding),
                Flat::Color::White, 1, false, 0.7f);

        }
        float bottom = window.getCameraBottom();

        window.writeText(scaleText, sf::Vector2f(left + padding, bottom + padding),
            Flat::Color::White, 1, false, 0.7f);
    }

    bool isOpen() {
        return window.isOpen();
    }

};

int main() {
    Math::Converter::setScale(1e11f);

    Simulation simulation;

    // Set Conversion Scale

    while (simulation.isOpen()) {
        simulation.update();
        simulation.draw();
    }

}
