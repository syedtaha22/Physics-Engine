#include <vector>
#include <exception>
#include <memory>
#include <sstream>
#include <iomanip>

// Include Flat Core
#include "Flat/core.hpp"

// Include Physics Core
#include "Engine/Physics/core.hpp"

// Include Math Core
#include "Engine/Math/core.hpp"

#include "Utils/StopWatch.hpp" // Include stopwatch utility


static constexpr float SunMass = 1.989e30f;
static constexpr float EarthMass = 5.972e24f;

static constexpr float SunRadius = 6.9634e8f; // meters
static constexpr float EarthRadius = 6.378e6f; // meters

static constexpr float EarthDistance = 147.12e9f;


class Game {
    Flat::Window window;

    std::vector<std::shared_ptr<Physics::Body>> bodies;

    std::vector<Flat::Color> colors;

    std::string scaleText;

public:
    Game() : window(1280, 768, "Physics Engine from Scratch", Flat::Color::Black, 5.0f, 32) {
        // Use ostringstream to convert the float to a string in scientific notation
        std::ostringstream oss;
        oss << std::scientific << std::setprecision(0) << Math::Converter::getScale();

        scaleText = "Scale: " + oss.str();


        // Add a Sun-Like Body
        bodies.push_back(Physics::Body::CreateCircularBody(SunRadius, SunMass, Math::Vector(0, 0)));
        colors.push_back(Flat::Color::Yellow);

        // Add an Earth-Like Body
        bodies.push_back(Physics::Body::CreateCircularBody(EarthRadius, EarthMass, Math::Vector(0, EarthDistance)));
        colors.push_back(Flat::Color::Blue);

    }

    void update() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Check for zoom
            if (event.type == (int)Flat::Event::MouseWheelScrolled) {
                if (event.mouseWheelScroll.delta > 0)
                    window.zoom(0.95f); // Zoom in
                else
                    window.zoom(1.05f); // Zoom out
            }


        }


        // stopwatch.start();
        // world.step(window.getElapsedTimeSinceLastFrame(Flat::TimeUnit::Seconds), 20);
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
        window.drawGridLines(1.0f, Flat::Color("#333333"));

        // Draw the bodies
        for (int i = 0; i < bodies.size(); i++) {
            auto body = bodies[i];

            // Get the position
            Math::Vector pos = body->getKinematicProperty("Position");
            float scale = Math::Converter::getScale();

            // Draw the body
            if (!body->physicalPropertyExists("Radius")) {
                std::cout << "Body does not have a radius" << std::endl;
                continue;
            }

            window.drawCircleWithBorder(body->getPhysicalProperty("Radius") / scale,
                Math::Converter::toVector2f(pos), colors[i], 0.1f, colors[i]);

        }

        displayScale();

        window.display();
    }

    void displayScale() {


        // Place the text on the bottom left corner
        // Get the left and bottom of the camera
        float left = window.getCameraLeft();
        float bottom = window.getCameraBottom();
        float padding = abs(window.getCameraRight() - left) * 0.03f;


        window.writeText(scaleText, sf::Vector2f(left + padding, bottom + padding),
            Flat::Color::White, 1, false);


    }

    bool isOpen() {
        return window.isOpen();
    }

};

int main() {
    Math::Converter::setScale(1e10f);

    Game game;

    // Set Conversion Scale

    while (game.isOpen()) {

        game.update();
        game.draw();
    }

}
