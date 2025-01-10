#include <vector>
#include <exception>

#define _USE_MATH_DEFINES
#include <cmath>


#include "Entity.hpp"
#include "FlatUtils/StopWatch.hpp" // Include stopwatch utility


// global stopwatch
FlatUtils::Stopwatch globalStopwatch;

class Game {
    Flat::Window window;

    FlatPhysics::FlatWorld world;

    std::vector<Entity> entities;

    FlatUtils::Stopwatch stopwatch;
    FlatUtils::Stopwatch sampleTimer;

    double totalStepTime = 0.0;
    int totalBodyCount = 0;
    int totalSampleCount = 0;
    std::string worldBodyCountString = "";
    std::string worldStepTimeString = "";

public:
    Game() : window(1280, 768, "Physics Engine from Scratch", Flat::Color::Black, 5.0f, 32) {
        initGround();

        initLedges();

        // Start the stopwatch
        sampleTimer.start();

    }

    void initGround() {
        // Get the camera extent
        float left, right, top, bottom; // Left, Right, Top, Bottom
        window.getCameraExtent(left, right, top, bottom);
        float padding = abs(right - left) * 0.1f;

        FlatPhysics::FlatBody* body = nullptr;
        // Make a ground object
        if (!FlatPhysics::FlatBody::createBoxBody(right - left - padding * 2, 3.0f, 10.f, true, 0.5f, body)) {
            throw std::runtime_error("Error creating ground body");
        }

        body->moveTo(FlatPhysics::FlatVector(0.0f, -10.f));
        // Add the body to the world
        world.addBody(body);
        entities.emplace_back(body, Flat::Color::DarkGreen);

    }

    void initLedges() {
        FlatPhysics::FlatBody* ledge = nullptr;

        // Add a ledge body
        if (!FlatPhysics::FlatBody::createBoxBody(20.0f, 2.0f, 1.0f, true, 0.5f, ledge)) {
            throw std::runtime_error("Error creating ledge body");
        }

        // Rotate the ledge
        ledge->rotate(-M_PI / 20.0f);
        ledge->moveTo(FlatPhysics::FlatVector(-15.0f, 5.0f));
        world.addBody(ledge);
        entities.emplace_back(ledge, Flat::Color::DarkBlue);


        // Add a ledge body
        if (!FlatPhysics::FlatBody::createBoxBody(20.0f, 2.0f, 1.0f, true, 0.5f, ledge)) {
            throw std::runtime_error("Error creating ledge body");
        }

        // Rotate the ledge
        ledge->rotate(M_PI / 20.0f);
        ledge->moveTo(FlatPhysics::FlatVector(13.0f, 10.0f));
        world.addBody(ledge);
        entities.emplace_back(ledge, Flat::Color::DarkBlue);
    }

    void addRandomBodyOfType(FlatPhysics::ShapeType type, FlatPhysics::FlatVector position) {
        FlatPhysics::FlatBody* body = nullptr;
        // If the left mouse button is clicked, add a circle
        if (type == FlatPhysics::ShapeType::Circle) {
            float radius = FlatUtils::Random::getRandomFloat(0.75f, 1.5f);
            entities.emplace_back(world, radius, false, position);
        }
        // If the Right mouse button is clicked, add a box
        if (type == FlatPhysics::ShapeType::Box) {
            float width = FlatUtils::Random::getRandomFloat(1.0f, 2.0f);
            float height = FlatUtils::Random::getRandomFloat(1.0f, 2.0f);

            entities.emplace_back(world, width, height, false, position);
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
                    window.zoom(0.95f); // Zoom in
                else
                    window.zoom(1.05f); // Zoom out
            }

            if (event.type == (int)Flat::Event::MouseButtonPressed) {
                FlatPhysics::FlatVector position =
                    FlatPhysics::FlatConverter::toFlatVector(Flat::Mouse::getPosition(window));

                // If the left mouse button is clicked, add a circle
                if (Flat::Mouse::isLeftButtonClicked()) {
                    addRandomBodyOfType(FlatPhysics::ShapeType::Circle, position);
                }
                // If the Right mouse button is clicked, add a box
                if (Flat::Mouse::isRightButtonClicked()) {
                    addRandomBodyOfType(FlatPhysics::ShapeType::Box, position);
                }
            }

            if (event.type == (int)Flat::Event::KeyPressed) {
                if (Flat::Keyboard::isKeyPressed(Flat::Key::S)) {
                    // Print the elapsed time between frames
                    std::cout << "Step Time: " << stopwatch.getElapsedTimeInMilliseconds() << " milliseconds" << std::endl;
                    std::cout << "Draw Time: " << globalStopwatch.getElapsedTimeInMilliseconds() << " milliseconds" << std::endl;
                    std::cout << "Internal Time: " << FlatPhysics::FlatWorld::worldStopwatch.getElapsedTimeInMilliseconds() << " milliseconds" << std::endl;
                    // Print the number of bodies
                    std::cout << "Number of bodies: " << world.numBodies() << std::endl;
                    std::cout << std::endl;

                }
            }
        }


        if (this->sampleTimer.getElapsedTimeInSeconds() > 1.0) {
            this->worldBodyCountString = "Body Count: " +
                FlatPhysics::FlatMath::RoundToString(this->totalBodyCount / (double)this->totalSampleCount, 4);
            this->worldStepTimeString = "Step Time: " +
                FlatPhysics::FlatMath::RoundToString(this->totalStepTime / (double)this->totalSampleCount, 4) + " ms";

            this->totalBodyCount = 0;
            this->totalStepTime = 0;
            this->totalSampleCount = 0;

            this->sampleTimer.restart();
        }

        stopwatch.start();
        world.step(window.getElapsedTimeSinceLastFrame(Flat::TimeUnit::Seconds), 20);
        stopwatch.stop();

        this->totalStepTime += stopwatch.getElapsedTimeInMilliseconds();
        this->totalBodyCount += world.numBodies();
        this->totalSampleCount++;

        float bottom = window.getCameraBottom();

        // Loop over the bodies
        for (int i = 0; i < world.numBodies(); i++) {
            FlatPhysics::FlatBody* body = nullptr;

            if (!world.getBody(i, body)) {
                throw std::runtime_error("Error getting body at index " + std::to_string(i));
            }

            // If the body is not static
            if (!body->isStatic) {
                // Get the position
                FlatPhysics::FlatAABB aabb = body->getAABB();
                //std::cout << "Body " << i << " AABB: " << aabb.min->x << ", " << aabb.min->y << " - " << aabb.max->x << ", " << aabb.max->y << std::endl;
                // Check if the body is out of bounds
                if (aabb.max->y < bottom) {
                    world.removeBody(i);
                    entities.erase(entities.begin() + i);
                }
            }
        }

        window.update();
    }


    void draw() {
        window.clear();
        window.drawGridLines(1.0f, Flat::Color("#333333"));

        for (size_t i = 0; i < world.numBodies(); i++)  entities[i].draw(window);
        displayStats();
        window.display();
    }

    void displayStats() {
        // Get the size of the string
        sf::Vector2f size = Flat::Text::getTextSize(this->worldBodyCountString, 1);

        // Place the text on the bottom left corner
        // Get the left and bottom of the camera
        float left = window.getCameraLeft();
        float bottom = window.getCameraBottom();
        float padding = abs(window.getCameraRight() - left) * 0.03f;


        window.writeText(this->worldBodyCountString, sf::Vector2f(left + padding, bottom + padding),
            Flat::Color::White, 1, false);
        window.writeText(this->worldStepTimeString, sf::Vector2f(left + padding, 0.5f + bottom + padding + size.y),
            Flat::Color::White, 1, false);

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
