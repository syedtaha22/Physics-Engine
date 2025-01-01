#include "Flat/core.hpp"

// Include  FlatPhysics core
#include "FlatPhysics/core.hpp"

// Include Random
#include "Random.hpp"

#include <vector>
#include <exception>


class Game {
    Flat::Window window;
    int numBodies = 10;
    std::vector<FlatPhysics::FlatBody*> bodies;
    std::vector<Flat::Color> colors;

public:
    Game() : window(1280, 768, "Physics Engine from Scratch", Flat::Color("#333333"), 0.08f, 32) {

        bodies.resize(numBodies);
        colors.resize(numBodies);

        // Get the camera extent
        float left, right, top, bottom; // Left, Right, Top, Bottom
        window.getCameraExtent(left, right, top, bottom);
        float padding = 5.0f;

        // Make a list of bodies
        const int numBodies = 10;
        bodies.resize(numBodies);
        colors.resize(numBodies);  // Colors for the bodies

        for (int i = 0;i < numBodies; i++) {
            int type = Random::randomInt(0, 1);

            type = 0;

            float x = Random::randomFloat(left + padding, right - padding);
            float y = Random::randomFloat(top + padding, bottom - padding);

            if (type == (int)FlatPhysics::ShapeType::Circle) {
                if (!FlatPhysics::FlatBody::createCircleBody(1.0f, FlatPhysics::FlatVector(x, y), 2, false, 0.1f, bodies[i])) {
                    // Throw error
                    throw std::runtime_error("Error creating circle body");
                }

            }
            else if (type == (int)FlatPhysics::ShapeType::Box) {
                if (!FlatPhysics::FlatBody::createBoxBody(3.0f, 3.0f, FlatPhysics::FlatVector(x, y), 2, false, 0.1f, bodies[i])) {
                    // Throw error
                    throw std::runtime_error("Error creating box body");
                }
            }
            else {
                // Throw error
                throw std::runtime_error("Invalid shape type");
            }

            // Assign a random color
            colors[i] = Random::randomColor();
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
                float speed = 0.15f;


                if (Flat::Keyboard::isKeyPressed(Flat::Key::Left)) dx = -speed;
                if (Flat::Keyboard::isKeyPressed(Flat::Key::Right)) dx = speed;
                if (Flat::Keyboard::isKeyPressed(Flat::Key::Up)) dy = -speed;
                if (Flat::Keyboard::isKeyPressed(Flat::Key::Down)) dy = speed;

                if (dx != 0 || dy != 0) {
                    FlatPhysics::FlatVector direction(dx, dy);
                    FlatPhysics::FlatMath::normalize(direction);

                    FlatPhysics::FlatVector velocity = direction * speed * window.getElapsedTime(Flat::TimeUnit::Seconds);
                    bodies[0]->move(velocity);
                }

            }
        }
    }


    void draw() {
        window.clear();

        for (size_t i = 0; i < numBodies; i++) {
            FlatPhysics::FlatBody* body = bodies[i];
            // Get the sf position
            sf::Vector2f pos = FlatPhysics::FlatConverter::toSFML(body->getPosition());

            // Check if the body is a circle
            if (body->shapeType == FlatPhysics::ShapeType::Circle) {
                window.drawCircleWithBorder(body->radius, pos, Flat::Color::White, 0.1, colors[i]);
            }
            else if (body->shapeType == FlatPhysics::ShapeType::Box) {
                window.drawRectangleFilled(body->width, body->height, pos, Flat::Color::Red);
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
