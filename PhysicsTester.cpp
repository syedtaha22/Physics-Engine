#include <vector>
#include <exception>
#include <memory>
#include <sstream>
#include <iomanip>
#include <unordered_map>

#define _USE_MATH_DEFINES
#include <cmath>


const double G = 6.67430e-11; // Gravitational constant

// Include Flat Core
#include "Flat/core.hpp"

// Include Physics Core
#include "Engine/Physics/core.hpp"

// Include Math Core
#include "Engine/Math/core.hpp"

#include "Utils/StopWatch.hpp" // Include stopwatch utility
#include "Utils/Random.hpp" // Include random utility


constexpr double RADIUS = 1e10; // Radius of the circle
constexpr double MASS = 5e23; // Mass of each body

class Game {
    Flat::Window window;

    Physics::World world;

    std::vector<Flat::Color> colors;

    std::string scaleText;

    bool started = false;
    bool showGrid = true;

    std::vector<Physics::Path> orbits;

public:
    Game() : window(1280, 768, "Physics Engine from Scratch", Flat::Color::Black, 5.0f, 32) {
        // Use ostringstream to convert the double to a string in scientific notation
        std::ostringstream oss;
        oss << std::scientific << std::setprecision(0) << Math::Converter::getScale();

        scaleText = "Scale: " + oss.str();


        // Add a Sun-Like Body
        world.addBody(Physics::Body::AstronomicalBody(5e26, Math::Vector(0, 0)));
        colors.push_back(Flat::Color::Yellow);

        // Add an Earth-Like Body
        add_bodies_around_circle(4, RADIUS, MASS);
        orbits.resize(world.numBodies());
    }

    // Function to add bodies around a circle
    void add_bodies_around_circle(int n, double radius, double mass) {
        // Get the mass of the central body
        double central_mass = world.getBody(0)->getPhysicalProperty("Mass");

        // Calculate the escape velocity just once
        double escape_velocity = sqrt(2 * G * central_mass / radius);

        for (int i = 0; i < n; ++i) {
            double angle = 2 * M_PI * i / n; // Angle for equidistant placement
            double x = radius * cos(angle); // X position
            double y = radius * sin(angle); // Y position

            double vx = -sin(angle) * (sqrt(G * 5e24 / radius)) * 5; // Tangential velocity (Vx)
            double vy = cos(angle) * (sqrt(G * 5e24 / radius)) * 5; // Tangential velocity (Vy)

            std::shared_ptr<Physics::Body> body = Physics::Body::AstronomicalBody(mass, Math::Vector(x, y));
            body->setKinematicProperty("LinearVelocity", Math::Vector(vx, vy));

            world.addBody(body); // Add body to the world
            colors.push_back(Utils::Random::getRandomColor()); // Add a random color
        }
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
            world.step(window.getElapsedTimeSinceLastFrame(Flat::TimeUnit::Seconds) * 1e6);
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
    Math::Converter::setScale(1e9f);

    Game game;

    // Set Conversion Scale

    while (game.isOpen()) {

        game.update();
        game.draw();
    }

}
