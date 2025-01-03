#include <vector>
#include <exception>

#define _USE_MATH_DEFINES
#include <cmath>

#include "Flat/core.hpp" // Include Flat core for graphics
#include "FlatPhysics/core.hpp" // Include  FlatPhysics core
#include "Random.hpp" // Include Random


class Game {
    Flat::Window window;
    int numBodies = 10;

    FlatPhysics::FlatWorld world;

    std::vector<Flat::Color> colors;
    std::vector<Flat::Color> outlineColors;

    std::vector<sf::Vector2f> vertexBuffer;

public:
    Game() : window(1280, 768, "Physics Engine from Scratch", Flat::Color("#333333"), 5.0f, 32) {

        colors.resize(numBodies);

        // Get the camera extent
        float left, right, top, bottom; // Left, Right, Top, Bottom
        window.getCameraExtent(left, right, top, bottom);
        float padding = 5.0f;

        // Make a list of bodies
        const int numBodies = 10;

        colors.resize(numBodies);  // Colors for the bodies
        outlineColors.resize(numBodies); // Outline colors for the bodies

        for (int i = 0;i < numBodies; i++) {
            int type = Random::randomInt(0, 1);

            FlatPhysics::FlatBody* body = nullptr;

            // type = (int)FlatPhysics::ShapeType::Circle;
            // type = (int)FlatPhysics::ShapeType::Box;

            float x = Random::randomFloat(left + padding, right - padding);
            float y = Random::randomFloat(top + padding, bottom - padding);


            if (type == (int)FlatPhysics::ShapeType::Circle) {
                if (!FlatPhysics::FlatBody::createCircleBody(1.0f, FlatPhysics::FlatVector(x, y), 2, false, 0.1f, body)) {
                    // Throw error
                    throw std::runtime_error("Error creating circle body");
                }

            }
            else if (type == (int)FlatPhysics::ShapeType::Box) {
                if (!FlatPhysics::FlatBody::createBoxBody(2.0f, 2.0f, FlatPhysics::FlatVector(x, y), 2, false, 0.1f, body)) {
                    // Throw error
                    throw std::runtime_error("Error creating box body");
                }
            }
            else {
                // Throw error
                throw std::runtime_error("Invalid shape type");
            }

            // Add the body to the world
            world.addBody(body);

            // Assign a random color
            colors[i] = Random::randomColor();
            outlineColors[i] = Flat::Color::White;
        }
    }


    void update() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Check for zoom
            if (event.type == (int)Flat::Event::MouseWheelScrolled) {
                if (event.mouseWheelScroll.delta > 0)
                    window.zoom(0.9f); // Zoom in
                else
                    window.zoom(1.1f); // Zoom out
            }

            if (event.type == (int)Flat::Event::KeyPressed) {


                float dx = 0;
                float dy = 0;
                float speed = 4.0f;

                //std::cout << "Key Pressed" << std::endl;

                FlatPhysics::FlatBody* body = nullptr;

                if (!world.getBody(0, body)) {
                    throw std::runtime_error("Error getting body");
                }


                if (Flat::Keyboard::isKeyPressed(Flat::Key::Left)) dx = -speed;
                if (Flat::Keyboard::isKeyPressed(Flat::Key::Right)) dx = speed;
                if (Flat::Keyboard::isKeyPressed(Flat::Key::Up)) dy = -speed;
                if (Flat::Keyboard::isKeyPressed(Flat::Key::Down)) dy = speed;

                if (dx != 0 || dy != 0) {
                    FlatPhysics::FlatVector direction(dx, dy);
                    FlatPhysics::FlatMath::Normalize(direction);



                    FlatPhysics::FlatVector velocity = direction * speed * window.getElapsedTimeSinceLastFrame(Flat::TimeUnit::Seconds);
                    body->move(velocity);
                }

            }
        }

        world.step(window.getElapsedTimeSinceLastFrame(Flat::TimeUnit::Seconds));

        window.update();
    }


    void draw() {
        window.clear();

        for (size_t i = 0; i < numBodies; i++) {
            FlatPhysics::FlatBody* body = nullptr;

            if (!world.getBody(i, body)) {
                throw std::runtime_error("Error getting body at index " + std::to_string(i));
            }


            // Get the sf position
            sf::Vector2f pos = FlatPhysics::FlatConverter::toVector2f(body->getPosition());

            // Check if the body is a circle
            if (body->shapeType == FlatPhysics::ShapeType::Circle) {
                window.drawCircleWithBorder(body->radius, pos, Flat::Color::White, 0.1, colors[i]);
            }
            else if (body->shapeType == FlatPhysics::ShapeType::Box) {

                FlatPhysics::FlatConverter::toVector2fArray(body->getTransformedVertices(), vertexBuffer);

                window.drawPolygonWithBorder(vertexBuffer, outlineColors[i], 0.1, colors[i]);
            }

        }

        window.display();
    }

    bool isOpen() {
        return window.isOpen();
    }

};

int main() {
    Game game;

    while (game.isOpen()) {
        game.update();
        game.draw();
    }

}
