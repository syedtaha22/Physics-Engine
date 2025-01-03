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
    std::vector<FlatPhysics::FlatBody*> bodies;
    std::vector<Flat::Color> colors;
    std::vector<Flat::Color> outlineColors;

    std::vector<sf::Vector2f> vertexBuffer;

public:
    Game() : window(1280, 768, "Physics Engine from Scratch", Flat::Color("#333333"), 5.0f, 32) {

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
        outlineColors.resize(numBodies); // Outline colors for the bodies

        for (int i = 0;i < numBodies; i++) {
            int type = Random::randomInt(0, 1);

            // type = (int)FlatPhysics::ShapeType::Circle;
            // type = (int)FlatPhysics::ShapeType::Box;

            float x = Random::randomFloat(left + padding, right - padding);
            float y = Random::randomFloat(top + padding, bottom - padding);


            if (type == (int)FlatPhysics::ShapeType::Circle) {
                if (!FlatPhysics::FlatBody::createCircleBody(1.0f, FlatPhysics::FlatVector(x, y), 2, false, 0.1f, bodies[i])) {
                    // Throw error
                    throw std::runtime_error("Error creating circle body");
                }

            }
            else if (type == (int)FlatPhysics::ShapeType::Box) {
                if (!FlatPhysics::FlatBody::createBoxBody(2.0f, 2.0f, FlatPhysics::FlatVector(x, y), 2, false, 0.1f, bodies[i])) {
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


                if (Flat::Keyboard::isKeyPressed(Flat::Key::Left)) dx = -speed;
                if (Flat::Keyboard::isKeyPressed(Flat::Key::Right)) dx = speed;
                if (Flat::Keyboard::isKeyPressed(Flat::Key::Up)) dy = -speed;
                if (Flat::Keyboard::isKeyPressed(Flat::Key::Down)) dy = speed;

                if (dx != 0 || dy != 0) {
                    FlatPhysics::FlatVector direction(dx, dy);
                    FlatPhysics::FlatMath::Normalize(direction);

                    FlatPhysics::FlatVector velocity = direction * speed * window.getElapsedTimeSinceLastFrame(Flat::TimeUnit::Seconds);
                    bodies[0]->move(velocity);
                }

            }
        }

        // Loop over all the bodies
        for (size_t i = 0; i < numBodies; i++) {
            //FlatPhysics::FlatBody* body = bodies[i];
            //body->rotate(M_PI / 2.0f * window.getElapsedTimeSinceLastFrame(Flat::TimeUnit::Seconds));
            outlineColors[i] = Flat::Color::White;
        }

        // Loop through all the bodies
        for (size_t i = 0; i < numBodies - 1; i++) {

            FlatPhysics::FlatBody* bodyA = bodies[i];

            for (size_t j = i + 1; j < numBodies; j++) {
                FlatPhysics::FlatBody* bodyB = bodies[j];
                FlatPhysics::FlatVector normal;
                float depth;


                if (bodyA->shapeType == FlatPhysics::ShapeType::Circle &&
                    bodyB->shapeType == FlatPhysics::ShapeType::Box) {

                    if (FlatPhysics::FlatCollisions::circlePolygonCollision(
                        bodyA->getPosition(), bodyA->radius,
                        bodyB->getTransformedVertices(), normal, depth)) {

                        // Set the outline colors to red (collision detected
                        outlineColors[i] = Flat::Color::Red;
                        outlineColors[j] = Flat::Color::Red;
                        // Move the bodies apart
                        bodyA->move(-normal * (depth / 2.0f));
                        bodyB->move(normal * (depth / 2.0f));
                    }

                }
                else if (bodyB->shapeType == FlatPhysics::ShapeType::Circle &&
                    bodyA->shapeType == FlatPhysics::ShapeType::Box) {

                    if (FlatPhysics::FlatCollisions::circlePolygonCollision(
                        bodyB->getPosition(), bodyB->radius,
                        bodyA->getTransformedVertices(), normal, depth)) {

                        // Set the outline colors to red (collision detected
                        outlineColors[i] = Flat::Color::Red;
                        outlineColors[j] = Flat::Color::Red;
                        // Move the bodies apart
                        bodyA->move(normal * (depth / 2.0f));
                        bodyB->move(-normal * (depth / 2.0f));
                    }

                }


#if false
                if (FlatPhysics::FlatCollisions::polygonPolygonCollision(
                    bodyA->getTransformedVertices(),
                    bodyB->getTransformedVertices(),
                    normal, depth)) {

                    // Set the outline colors to red (collision detected
                    outlineColors[i] = Flat::Color::Red;
                    outlineColors[j] = Flat::Color::Red;
                    // Move the bodies apart
                    bodyA->move(-normal * (depth / 2.0f));
                    bodyB->move(normal * (depth / 2.0f));
                }



                if (FlatPhysics::FlatCollisions::circleCircleCollision(
                    bodyA->getPosition(), bodyA->radius,
                    bodyB->getPosition(), bodyB->radius,
                    normal, depth)) {

                    // Move the bodies apart
                    bodyA->move(-normal * (depth / 2.0f));
                    bodyB->move(normal * (depth / 2.0f));
                }
#endif
            }
        }

        window.update();


    }


    void draw() {
        window.clear();

        for (size_t i = 0; i < numBodies; i++) {
            FlatPhysics::FlatBody* body = bodies[i];
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
