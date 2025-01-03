#include <algorithm>
#include <stdexcept>

#include "../headers/FlatWorld.hpp"
#include "../headers/FlatBody.hpp"
#include "../headers/FlatVector.hpp"
#include "../headers/FlatCollisions.hpp"


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
        bodies[i]->step(time);
    }

    // Collide
            // Loop through all the bodies
    for (size_t i = 0; i < bodies.size() - 1; i++) {
        FlatPhysics::FlatBody* bodyA = bodies[i];

        for (size_t j = i + 1; j < bodies.size(); j++) {
            FlatPhysics::FlatBody* bodyB = bodies[j];
            FlatPhysics::FlatVector normal;
            float depth;

            if (collides(bodyA, bodyB, normal, depth)) {
                // Move the bodies apart
                bodyA->move(-normal * (depth / 2.0f));
                bodyB->move(normal * (depth / 2.0f));
            }

        }
    }

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
                bodyB->getTransformedVertices(), normal, depth);

        }
    }
    else if (shapeA == ShapeType::Box) {
        if (shapeB == ShapeType::Circle) {
            bool result = FlatCollisions::circlePolygonCollision(bodyB->getPosition(), bodyB->radius,
                bodyA->getTransformedVertices(), normal, depth);

            normal = -normal; // Reverse the normal
            return result;

        }
        else if (shapeB == ShapeType::Box) {
            return FlatCollisions::polygonPolygonCollision(bodyA->getTransformedVertices(),
                bodyB->getTransformedVertices(), normal, depth);
        }
    }

    return false;
}