#include <iostream>
#include <algorithm>
#include <stdexcept>

#include "../headers/FlatWorld.hpp"
#include "../headers/FlatBody.hpp"
#include "../headers/FlatVector.hpp"
#include "../headers/FlatCollisions.hpp"
#include "../headers/FlatMath.hpp"


FlatPhysics::FlatVector FlatPhysics::FlatWorld::Gravity = FlatPhysics::FlatVector(0.0f, -9.8f);

FlatPhysics::FlatWorld::~FlatWorld() {
    for (size_t i = 0; i < bodies.size(); i++) {
        delete bodies[i];
    }
}

void FlatPhysics::FlatWorld::addBody(FlatBody* body) {
    bodies.push_back(body);
}

void FlatPhysics::FlatWorld::removeBody(int index) {
    if (index >= 0 && index < bodies.size()) {
        bodies.erase(bodies.begin() + index);
    }
    else {
        throw std::out_of_range("Index out of bounds.");
    }
}

bool FlatPhysics::FlatWorld::getBody(int index, FlatBody*& body) {
    if (index >= 0 && index < bodies.size()) {
        body = bodies[index];
        return true;
    }
    return false;
}

void FlatPhysics::FlatWorld::step(float time) {

    // Move
    for (size_t i = 0; i < bodies.size(); i++) {
        bodies[i]->step(time, FlatWorld::Gravity);
    }

    // Collide
    for (size_t i = 0; i < bodies.size() - 1; i++) {
        FlatPhysics::FlatBody* bodyA = bodies[i];

        for (size_t j = i + 1; j < bodies.size(); j++) {
            FlatPhysics::FlatBody* bodyB = bodies[j];
            FlatPhysics::FlatVector normal;
            float depth;

            // Check if both bodies are static
            if (bodyA->isStatic && bodyB->isStatic) {
                continue;
            }

            if (collides(bodyA, bodyB, normal, depth)) {
                if (bodyA->isStatic) bodyB->move(normal * depth); // If body A is static, move body B
                else if (bodyB->isStatic) bodyA->move(-normal * depth);// If body B is static, move body A
                else { // If both are dynamic, move both
                    bodyA->move(-normal * (depth / 2.0f));
                    bodyB->move(normal * (depth / 2.0f));
                }

                // Resolve the collision
                resolveCollisions(bodyA, bodyB, normal, depth);
            }

        }
    }
}

void FlatPhysics::FlatWorld::resolveCollisions(FlatBody*& bodyA, FlatBody*& bodyB,
    FlatVector& normal, float depth)
{
    // Calculate Relative velocity
    FlatVector relativeVelocity = bodyB->linearVelocity - bodyA->linearVelocity;

    if (FlatMath::DotProduct(relativeVelocity, normal) > 0) {
        return;
    }

    // Calculate restitution
    float e = std::min(bodyA->restitution, bodyB->restitution);

    // Calculate Impulse magnitude
    float impulseMagnitude = -(1 + e) * FlatMath::DotProduct(relativeVelocity, normal);
    impulseMagnitude /= (bodyA->inverseMass + bodyB->inverseMass);

    FlatVector impulse = impulseMagnitude * normal;

    bodyA->linearVelocity -= impulse * bodyA->inverseMass;
    bodyB->linearVelocity += impulse * bodyB->inverseMass;
}

bool FlatPhysics::FlatWorld::collides(FlatBody*& bodyA, FlatBody*& bodyB,
    FlatVector& normal, float& depth)
{
    normal = FlatVector::Zero;
    depth = 0.0f;

    ShapeType shapeA = bodyA->shapeType;
    ShapeType shapeB = bodyB->shapeType;

    if (shapeA == ShapeType::Circle) {
        if (shapeB == ShapeType::Circle) {
            return FlatCollisions::circleCircleCollision(bodyA->getPosition(), bodyA->radius,
                bodyB->getPosition(), bodyB->radius, normal, depth);
        }
        else if (shapeB == ShapeType::Box) {
            return FlatCollisions::circlePolygonCollision(bodyA->getPosition(), bodyA->radius,
                bodyB->getPosition(), bodyB->getTransformedVertices(), normal, depth);
        }
    }
    else if (shapeA == ShapeType::Box) {
        if (shapeB == ShapeType::Circle) {
            bool result = FlatCollisions::circlePolygonCollision(bodyB->getPosition(), bodyB->radius,
                bodyA->getPosition(), bodyA->getTransformedVertices(), normal, depth);
            normal = -normal; // Reverse the normal
            return result;
        }
        else if (shapeB == ShapeType::Box) {
            return FlatCollisions::polygonPolygonCollision(
                bodyA->getPosition(), bodyA->getTransformedVertices(),
                bodyB->getPosition(), bodyB->getTransformedVertices(),
                normal, depth);
        }
    }

    return false;
}

size_t FlatPhysics::FlatWorld::numBodies() const {
    return bodies.size();
}

void FlatPhysics::FlatWorld::clear() {
    for (size_t i = 0; i < bodies.size(); i++) {
        delete bodies[i];
    }
    bodies.clear();
    std::cout << "World Cleared" << std::endl;
    std::cout << "Number of Bodies: " << bodies.size() << std::endl;
}