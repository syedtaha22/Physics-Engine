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

void FlatPhysics::FlatWorld::step(float time, size_t totalIterations) {
    totalIterations = FlatMath::Clamp<size_t>(totalIterations, MinIterations, MaxIterations);

    FlatWorld::worldStopwatch.start();
    for (size_t currentIteration = 0; currentIteration < totalIterations; currentIteration++) {
        // Move
        stepBodies(time, totalIterations);

        // Broad Phase: Find potential collisions
        findPotentialCollisions();

        // Narrow Phase : Resolve potential collisions
        resolvePotentialCollisions();
    }
    FlatWorld::worldStopwatch.stop();
}


void FlatPhysics::FlatWorld::stepBodies(float time, size_t totalIterations) {
    // Move the bodies
    for (size_t i = 0; i < bodies.size(); i++) bodies[i]->step(time / totalIterations, Gravity);
}

void FlatPhysics::FlatWorld::findPotentialCollisions() {
    // Check if two bodies are close enough to collide

    // Initialize the Pointer to the bodies
    FlatBody* bodyA = nullptr;
    FlatBody* bodyB = nullptr;

    // Clear the potential collisions for the current step
    potentialCollisions.clear();

    for (size_t i = 0; i < bodies.size() - 1; i++) {
        bodyA = bodies[i];
        FlatAABB bodyA_AABB = bodyA->getAABB();

        for (size_t j = i + 1; j < bodies.size(); j++) {
            bodyB = bodies[j];
            FlatAABB bodyB_AABB = bodyB->getAABB();

            // Check if both bodies are static
            if (bodyA->isStatic && bodyB->isStatic) continue;

            // If the AABBs do not collide, skip the collision check
            if (!FlatCollisions::AABB_AABB_Collision(bodyA_AABB, bodyB_AABB)) continue;

            potentialCollisions.push_back(std::make_pair(i, j));
        }
    }
}

void FlatPhysics::FlatWorld::resolvePotentialCollisions() {
    // Define a Resolver Object, that will store the information about the collision
    FlatContactResolver resolver;

    // Resolve Collisions
    for (size_t i = 0; i < potentialCollisions.size(); i++) {
        // Set the resolver bodies
        resolver.bodyA = bodies[potentialCollisions[i].first];
        resolver.bodyB = bodies[potentialCollisions[i].second];

        // Check if the bodies collide, and get the resolution information
        if (FlatCollisions::collides(resolver.bodyA, resolver.bodyB, resolver.normal, resolver.depth)) {

            // Seperate the bodies
            seperateBodies(resolver.bodyA, resolver.bodyB, resolver.normal * resolver.depth);

            // Get the collision points and counts
            FlatCollisions::getCollisionPoints(resolver);

            // Resolve the collision
            resolveCollisionsWithRotation(resolver);
        }
    }
}

void FlatPhysics::FlatWorld::seperateBodies(FlatBody* bodyA, FlatBody* bodyB,
    const FlatVector& minimumTranslationVector)
{
    // If body A is static, move body B
    if (bodyA->isStatic) bodyB->move(minimumTranslationVector);
    // If body B is static, move body A
    else if (bodyB->isStatic) bodyA->move(-minimumTranslationVector);
    // If both are dynamic, move both
    else {
        bodyA->move(-minimumTranslationVector / 2.0f);
        bodyB->move(minimumTranslationVector / 2.0f);
    }
}

void FlatPhysics::FlatWorld::resolveCollisionsBasic(const FlatContactResolver& resolver)
{

    FlatBody* bodyA = resolver.bodyA;
    FlatBody* bodyB = resolver.bodyB;
    FlatVector normal = resolver.normal;
    float depth = resolver.depth;


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

void FlatPhysics::FlatWorld::resolveCollisionsWithRotation(FlatContactResolver& resolver)
{

    // Calculate restitution
    resolver.e = std::min(resolver.bodyA->restitution, resolver.bodyB->restitution);
    FlatVector raPerp;
    FlatVector rbPerp;
    FlatVector angularLinearVelocityA;
    FlatVector angularLinearVelocityB;


    for (size_t i = 0; i < resolver.contactCount; i++) {
        resolver.Ra[i] = resolver.contactList[i] - resolver.bodyA->getPosition();
        resolver.Rb[i] = resolver.contactList[i] - resolver.bodyB->getPosition();

        raPerp = FlatMath::Perpendicular(resolver.Ra[i]);
        rbPerp = FlatMath::Perpendicular(resolver.Rb[i]);

        angularLinearVelocityA = resolver.bodyA->angularVelocity * raPerp;
        angularLinearVelocityB = resolver.bodyB->angularVelocity * rbPerp;

        FlatVector relativeVelocity =
            (resolver.bodyB->linearVelocity + angularLinearVelocityB) -
            (resolver.bodyA->linearVelocity + angularLinearVelocityA);

        float dotProduct = FlatMath::DotProduct(relativeVelocity, resolver.normal);

        if (dotProduct > 0) continue;

        float raDotN = FlatMath::DotProduct(resolver.Ra[i], resolver.normal);
        float rbDotN = FlatMath::DotProduct(resolver.Rb[i], resolver.normal);

        float denominator = resolver.bodyA->inverseMass + resolver.bodyB->inverseMass +
            (raDotN * raDotN * resolver.bodyA->inverseInertia) +
            (rbDotN * rbDotN * resolver.bodyB->inverseInertia);

        float j = -(1 + resolver.e) * dotProduct;
        j /= denominator;
        j /= (float)resolver.contactCount;

        resolver.impulses[i] = resolver.normal * j;
    }

    for (size_t i = 0; i < resolver.contactCount; i++) {
        resolver.bodyA->linearVelocity += -resolver.impulses[i] * resolver.bodyA->inverseMass;
        resolver.bodyA->angularVelocity += -FlatMath::CrossProduct(resolver.Ra[i],
            resolver.impulses[i]) * resolver.bodyA->inverseInertia;

        resolver.bodyB->linearVelocity += resolver.impulses[i] * resolver.bodyB->inverseMass;
        resolver.bodyB->angularVelocity += FlatMath::CrossProduct(resolver.Rb[i],
            resolver.impulses[i]) * resolver.bodyB->inverseInertia;
    }
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