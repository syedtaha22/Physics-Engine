#include <algorithm>

#include "../headers/World.hpp"
#include "../headers/Body.hpp"
#include "../headers/Constants.hpp"

#include "../../Math/headers/Operation.hpp"
#include "../../Math/headers/Vector.hpp"


namespace Physics {

    void World::addBody(std::shared_ptr<Body> body) {
        bodies.push_back(body);
    }

    void World::removeBody(std::shared_ptr<Body> body) {
        auto it = std::find(bodies.begin(), bodies.end(), body);
        if (it != bodies.end()) {
            bodies.erase(it);
        }
    }

    std::shared_ptr<Body> World::getBody(int index) {
        if (index >= 0 && index < bodies.size()) {
            return bodies[index];
        }
        return nullptr;
    }

    size_t World::numBodies() const {
        return bodies.size();
    }

    void World::step(double time) {
        calculateBodyAccelerations();
        for (size_t i = 0; i < bodies.size(); i++) {
            bodies[i]->step(time);
        }
    }

    void World::calculateBodyAccelerations() {
        /*
            Calculate the Gravitational Acceleration acting on each body

            From Newton's Law of Universal Gravitation:
            F = G * (m1 * m2) / r^2
                => F = m1 * a1
            m1 * a1 = G * (m1 * m2) / r^2

            => a1 = G * m2 / r^2

            Where:
                G = Gravitational Constant
                a1 = Acceleration acting on the first body due to the second body
                m2 = Mass of the second body
                r = Distance between the two bodies

            For the direction of the acceleration, calculate the unit vector in the direction
            of the second body from the first body and multiply it by the magnitude of the acceleration
        */
        for (size_t i = 0; i < bodies.size(); i++) {
            bodies[i]->setKinematicProperty("Acceleration", Math::Vector::Zero);
            Math::Vector position = bodies[i]->getKinematicProperty("Position");

            for (size_t j = 0; j < bodies.size(); j++) {
                if (i == j) continue;

                Math::Vector direction = bodies[j]->getKinematicProperty("Position") - position;

                double distance = Math::Operation::Length(direction);
                direction = direction / distance;

                double mass = bodies[j]->getPhysicalProperty("Mass");

                double accelerationMagnitude = Constants::GRAVITATIONAL_CONSTANT * mass / (distance * distance);
                Math::Vector acceleration = direction * accelerationMagnitude;

                bodies[i]->addKinematicProperty("Acceleration", acceleration);
            }
        }
    }

} // namespace Physics