/*

    Holds information about a body and it's color

*/

#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <stdexcept>

#include "FlatPhysics/core.hpp"
#include "Flat/core.hpp"
#include "FlatUtils/Random.hpp"



class Entity {
    FlatPhysics::FlatBody* body;
    Flat::Color color;

public:

    Entity(FlatPhysics::FlatBody* body, Flat::Color color) : body(body), color(color) {}
    Entity(FlatPhysics::FlatBody* body) : body(body), color(FlatUtils::Random::getRandomColor()) {}

    Entity(FlatPhysics::FlatWorld& world, float radius, float isStatic, FlatPhysics::FlatVector position) {
        if (!FlatPhysics::FlatBody::createCircleBody(radius, 2.0f, isStatic, 0.6f, body)) {
            throw std::runtime_error("Error creating circle body");
        }

        body->moveTo(position);
        world.addBody(body);
        color = FlatUtils::Random::getRandomColor();
    }

    Entity(FlatPhysics::FlatWorld& world, float width, float height, float isStatic, FlatPhysics::FlatVector position) {
        if (!FlatPhysics::FlatBody::createBoxBody(width, height, 2.0f, isStatic, 0.6f, body)) {
            throw std::runtime_error("Error creating box body");
        }

        body->moveTo(position);
        world.addBody(body);
        color = FlatUtils::Random::getRandomColor();
    }


    FlatPhysics::FlatBody* getBody() {
        return body;
    }

    Flat::Color getColor() {
        return color;
    }

    void setColor(Flat::Color color) {
        this->color = color;
    }

    void setBody(FlatPhysics::FlatBody* body) {
        this->body = body;
    }

    void draw(Flat::Window& window) {
        FlatPhysics::FlatVector position = body->getPosition();
        sf::Vector2f pos = FlatPhysics::FlatConverter::toVector2f(position);

        if (body->shapeType == FlatPhysics::ShapeType::Circle) {
            window.drawCircleWithBorder(body->radius, pos, Flat::Color::White, 0.05f, color);
        }
        else if (body->shapeType == FlatPhysics::ShapeType::Box) {
            window.drawRectangleWithBorder(body->width, body->height, body->getAngle(),
                pos, Flat::Color::White, 0.05f, color);
        }
    }

    ~Entity() {
        // The world class handles the deletion of the body
        body = nullptr;
    }

};

#endif // ENTITY_HPP    