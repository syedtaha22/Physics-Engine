#include <iostream>
#include <algorithm>
#include <stdexcept>

#include "../headers/FlatWorld.hpp"
#include "../headers/FlatBody.hpp"
#include "../headers/FlatVector.hpp"
#include "../headers/FlatCollisions.hpp"
#include "../headers/FlatMath.hpp"
#include "../headers/FlatAABB.hpp"


FlatPhysics::FlatVector FlatPhysics::FlatWorld::Gravity = FlatPhysics::FlatVector(0.0f, -9.8f);
FlatUtils::Stopwatch FlatPhysics::FlatWorld::worldStopwatch = FlatUtils::Stopwatch();

FlatPhysics::FlatWorld::~FlatWorld() {
    for (size_t i = 0; i < bodies.size(); i++) {
        delete bodies[i];
    }
}

void FlatPhysics::FlatWorld::addBody(FlatBody* body) {
    bodies.push_back(body);
}

void FlatPhysics::FlatWorld::removeBody(int index) {
    if (index < 0 || index >= bodies.size()) {
        throw std::out_of_range("Index out of bounds.");
    }

    // Delete the body
    delete bodies[index];

    // Remove the body from the list of bodies
    bodies.erase(bodies.begin() + index);
}

bool FlatPhysics::FlatWorld::getBody(int index, FlatBody*& body) {
    if (index >= 0 && index < bodies.size()) {
        body = bodies[index];
        return true;
    }
    return false;
}

void FlatPhysics::FlatWorld::step(float time, size_t iterations) {
    iterations = FlatMath::Clamp<size_t>(iterations, MinIterations, MaxIterations);

    // Initilize normal and depth, only once instead of in each loop
    FlatPhysics::FlatVector normal = FlatPhysics::FlatVector::Zero;
    float depth = 0.0f;

    // Make pointers to the bodies, once instead of in each loop
    FlatPhysics::FlatBody* bodyA = nullptr;
    FlatPhysics::FlatBody* bodyB = nullptr;

    // Make contact points, only once instead of in each loop
    FlatVector contact1 = FlatVector::Zero;
    FlatVector contact2 = FlatVector::Zero;
    int contactCount = 0;

    // boolean for printing
    bool print = true;

    // Clear the contact points
    contactPoints.clear();


    FlatWorld::worldStopwatch.start();
    for (size_t it = 0; it < iterations; it++) {
        // Move
        for (size_t i = 0; i < bodies.size(); i++) {
            bodies[i]->step(time / iterations, Gravity);
        }

        // Clear the contact list
        contactList.clear();

        // Collide
        for (size_t i = 0; i < bodies.size() - 1; i++) {
            bodyA = bodies[i];
            FlatAABB bodyA_AABB = bodyA->getAABB();

            for (size_t j = i + 1; j < bodies.size(); j++) {
                bodyB = bodies[j];
                FlatAABB bodyB_AABB = bodyB->getAABB();

                if (!FlatCollisions::AABB_AABB_Collision(bodyA_AABB, bodyB_AABB)) continue;


                // Check if both bodies are static
                if (bodyA->isStatic && bodyB->isStatic) continue;


                if (FlatCollisions::collides(bodyA, bodyB, normal, depth)) {
                    if (bodyA->isStatic) bodyB->move(normal * depth); // If body A is static, move body B
                    else if (bodyB->isStatic) bodyA->move(-normal * depth);// If body B is static, move body A
                    else { // If both are dynamic, move both
                        bodyA->move(-normal * (depth / 2.0f));
                        bodyB->move(normal * (depth / 2.0f));
                    }

                    FlatCollisions::getCollisionPoints(bodyA, bodyB, contact1, contact2, contactCount);
                    contactList.emplace_back(bodyA, bodyB, depth, normal, contact1, contact2, contactCount);
                }


            }

        }
        // Resolve Collisions
        for (size_t i = 0; i < contactList.size(); i++) {
            resolveCollisions(contactList[i]);

            if (contactList[i].contactCount > 0) {
                // See if the contact1 is already in the list
                if (std::find(contactPoints.begin(), contactPoints.end(),
                    contactList[i].contact1) == contactPoints.end())
                    contactPoints.push_back(contactList[i].contact1);

                if (contactList[i].contactCount > 1) {
                    // See if the contact2 is already in the list
                    if (std::find(contactPoints.begin(), contactPoints.end(),
                        contactList[i].contact2) == contactPoints.end())
                        contactPoints.push_back(contactList[i].contact2);
                }
            }
        }
    }
    FlatWorld::worldStopwatch.stop();
}

void FlatPhysics::FlatWorld::resolveCollisions(const FlatManifold& collisionManifold)
{

    FlatBody* bodyA = collisionManifold.bodyA;
    FlatBody* bodyB = collisionManifold.bodyB;
    FlatVector normal = collisionManifold.normal;
    float depth = collisionManifold.depth;


    // Relative velocity = velocity of B - velocity of A
    // Get the dot product of relative velocity and normal
    float dotProduct = FlatMath::DotProduct(bodyB->linearVelocity - bodyA->linearVelocity, normal);

    // If the dot product is greater than 0, the bodies are moving away from each other
    // No need to resolve the collision
    if (dotProduct > 0) return;

    // Calculate restitution
    float e = std::min(bodyA->restitution, bodyB->restitution);

    /*
                            - (1 + e) * RelativeVelocity ∙ Normal
        Impulse Magnitude = ------------------------------------
                                    1 / massA + 1 / massB

        Simplified to:
        dotProduct = RelativeVelocity ∙ Normal

                                 - (1 + e) * dotProduct
        Impulse Magnitude = -------------------------------
                              InverseMassA + InverseMassB

        And then the Impulse
        Impulse = Impulse Magnitude * Normal
    */

    // Calculate Impulse 
    FlatVector impulse = -(1 + e) * dotProduct * normal;
    impulse /= (bodyA->inverseMass + bodyB->inverseMass);

    bodyA->linearVelocity -= impulse * bodyA->inverseMass;
    bodyB->linearVelocity += impulse * bodyB->inverseMass;
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