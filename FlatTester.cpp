#include <vector>
#include <exception>

#define _USE_MATH_DEFINES
#include <cmath>

#include "Flat/core.hpp" // Include Flat core for graphics
#include "FlatPhysics/core.hpp" // Include  FlatPhysics core
#include "FlatUtils/Random.hpp" // Include random utility
#include "FlatUtils/StopWatch.hpp" // Include stopwatch utility


// global stopwatch
FlatUtils::Stopwatch globalStopwatch;


class Game {
    Flat::Window window;

    FlatPhysics::FlatWorld world;

    std::vector<Flat::Color> colors;
    std::vector<Flat::Color> outlineColors;

    FlatUtils::Stopwatch stopwatch;

    std::vector<sf::Vector2f> vertexBuffer;

public:
    Game() : window(1280, 768, "Physics Engine from Scratch", Flat::Color("#333333"), 5.0f, 32) {


        // Get the camera extent
        float left, right, top, bottom; // Left, Right, Top, Bottom
        window.getCameraExtent(left, right, top, bottom);
        float padding = abs(right - left) * 0.1f;


        FlatPhysics::FlatBody* body = nullptr;

        // Make a ground object
        if (!FlatPhysics::FlatBody::createBoxBody(right - left - padding * 2, 3.0f,
            FlatPhysics::FlatVector(0.0f, -10.f), 10.f, true, 0.5f, body)) {
            throw std::runtime_error("Error creating ground body");
        }


        // Add the body to the world
        world.addBody(body);

        colors.push_back(Flat::Color::Green);
        outlineColors.push_back(Flat::Color::White);

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

            if (event.type == (int)Flat::Event::MouseButtonPressed) {
                FlatPhysics::FlatVector position = FlatPhysics::FlatConverter::toFlatVector(Flat::Mouse::getPosition(window));

                // If the left mouse button is clicked, add a circle
                if (Flat::Mouse::isLeftButtonClicked()) {
                    float radius = FlatUtils::Random::randomFloat(0.75f, 1.5f);
                    FlatPhysics::FlatVector position = FlatPhysics::FlatConverter::toFlatVector(Flat::Mouse::getPosition(window));

                    FlatPhysics::FlatBody* body = nullptr;

                    if (!FlatPhysics::FlatBody::createCircleBody(radius, position, 2.0f, false, 0.6f, body)) {
                        throw std::runtime_error("Error creating circle body");
                    }

                    world.addBody(body);
                    colors.push_back(FlatUtils::Random::randomColor());
                    outlineColors.push_back(Flat::Color::White);
                }
                // If the Right mouse button is clicked, add a box
                if (Flat::Mouse::isRightButtonClicked()) {
                    float width = FlatUtils::Random::randomFloat(1.0f, 2.0f);
                    float height = FlatUtils::Random::randomFloat(1.0f, 2.0f);

                    FlatPhysics::FlatBody* body = nullptr;


                    if (!FlatPhysics::FlatBody::createBoxBody(width, height, position, 2.0f, false, 0.6f, body)) {
                        throw std::runtime_error("Error creating box body");
                    }

                    world.addBody(body);
                    colors.push_back(FlatUtils::Random::randomColor());
                    outlineColors.push_back(Flat::Color::White);
                }
            }


            if (event.type == (int)Flat::Event::KeyPressed) {
                if (Flat::Keyboard::isKeyPressed(Flat::Key::S)) {
                    // Print the elapsed time between frames
                    std::cout << "Step Time: " << stopwatch.getElapsedTimeInMilliseconds() << " milliseconds" << std::endl;
                    std::cout << "Draw Time: " << globalStopwatch.getElapsedTimeInMilliseconds() << " milliseconds" << std::endl;
                    std::cout << "Internal Time: " << FlatPhysics::FlatWorld::worldStopwatch.getElapsedTimeInMilliseconds() << " milliseconds" << std::endl;

                    // Print the number of allocated and deallocated objects
                    std::cout << std::endl;

                    // Print the number of bodies
                    std::cout << "Number of bodies: " << world.numBodies() << std::endl;
                    std::cout << std::endl;

                }




#if false  
                float dx = 0;
                float dy = 0;
                float forceMagnitude = 40.0f;
                FlatPhysics::FlatBody* body = nullptr;

                if (!world.getBody(0, body)) throw std::runtime_error("Error getting body");
                if (Flat::Keyboard::isKeyPressed(Flat::Key::Left)) dx--;
                if (Flat::Keyboard::isKeyPressed(Flat::Key::Right)) dx++;
                if (Flat::Keyboard::isKeyPressed(Flat::Key::Up)) dy--;
                if (Flat::Keyboard::isKeyPressed(Flat::Key::Down)) dy++;
                if (dx != 0 || dy != 0) {
                    FlatPhysics::FlatVector direction(dx, dy);
                    direction = FlatPhysics::FlatMath::Normalize(direction);
                    FlatPhysics::FlatVector force = direction * forceMagnitude;
                    body->applyForce(force);
                }
#endif
            }
        }

        stopwatch.start();
        world.step(window.getElapsedTimeSinceLastFrame(Flat::TimeUnit::Seconds), 20);
        stopwatch.stop();

        float bottom = window.getCameraBottom();

        // Loop over the bodies
        for (int i = 0; i < world.numBodies(); i++) {
            FlatPhysics::FlatBody* body = nullptr;

            if (!world.getBody(i, body)) {
                throw std::runtime_error("Error getting body at index " + std::to_string(i));
            }

            // Get the position
            FlatPhysics::FlatAABB aabb = body->getAABB();
            //std::cout << "Body " << i << " AABB: " << aabb.min->x << ", " << aabb.min->y << " - " << aabb.max->x << ", " << aabb.max->y << std::endl;
            // Check if the body is out of bounds
            if (aabb.max->y < bottom) {
                world.removeBody(i);
                colors.erase(colors.begin() + i);
                outlineColors.erase(outlineColors.begin() + i);
            }
        }

        window.update();
    }


    void draw() {
        window.clear();
        window.drawGridLines(1.0f, 0.1f, Flat::Color(255, 255, 255, 50));

        for (size_t i = 0; i < world.numBodies(); i++) {
            FlatPhysics::FlatBody* body = nullptr;

            if (!world.getBody(i, body)) {
                throw std::runtime_error("Error getting body at index " + std::to_string(i));
            }


            // Get the sf position
            sf::Vector2f pos = FlatPhysics::FlatConverter::toVector2f(body->getPosition());

            // Check if the body is a circle
            if (body->shapeType == FlatPhysics::ShapeType::Circle) {
                window.drawCircleWithBorder(body->radius, pos, outlineColors[i], 0.05f, colors[i]);
            }
            else if (body->shapeType == FlatPhysics::ShapeType::Box) {

                FlatPhysics::FlatConverter::toVector2fArray(body->getTransformedVertices(), vertexBuffer);

                window.drawPolygonWithBorder(vertexBuffer, outlineColors[i], 0.05f, colors[i]);
            }

        }

        window.display();
    }

    bool isOpen() {
        return window.isOpen();
    }

    void wrapScreen() {

        // Get the camera extent
        float left, right, top, bottom; // Left, Right, Top, Bottom
        window.getCameraExtent(left, right, top, bottom);


        // Loop over the bodies
        for (int i = 0; i < world.numBodies(); i++) {
            FlatPhysics::FlatBody* body = nullptr;

            if (!world.getBody(i, body)) {
                throw std::runtime_error("Error getting body at index " + std::to_string(i));
            }

            // Get the position
            FlatPhysics::FlatVector pos = body->getPosition();

            // Check if the body is out of bounds, and wrap it
            if (pos.x < left) pos.x = right;
            if (pos.x > right) pos.x = left;
            if (pos.y < top) pos.y = bottom;
            if (pos.y > bottom) pos.y = top;

            // Set the new position
            body->moveTo(pos);


        }
    }

};

int main() {
    Game game;

    while (game.isOpen()) {

        game.update();
        globalStopwatch.start();
        game.draw();
        globalStopwatch.stop();
    }

}
