
#include <algorithm>

#include "../headers/World.hpp"
#include "../headers/Body.hpp"
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
        calculateGForces();
        for (size_t i = 0; i < bodies.size(); i++) {
            bodies[i]->step(time);
        }
    }

    void World::calculateGForces() {
        // Loop over each body, and calculate the gravitational forces
        for (size_t i = 0; i < bodies.size() - 1; i++) {
            for (size_t j = i + 1; j < bodies.size(); j++) {
                calculateGravitionalForce(bodies[i], bodies[j]);
            }
        }
    }

    void World::calculateGravitionalForce(std::shared_ptr<Body> body1, std::shared_ptr<Body> body2) {

        /*
            Newton's Law of Universal Gravitation:
            F_Mag = G * (m1 * m2) / r^2

            F_Dir = F_Mag * (r2 - r1) / r



            Where:
            F_Mag = Magnitude of the Force
            F_Dir = Direction of the Force
            G = Gravitational Constant
            m1, m2 = Masses of the bodies
            r = Distance between the centers of the bodies
            r1 = Position of the first body
            r2 = Position of the second body

            This gives the force applied by the First body on the Second body
        */
        // Get the positions of the bodies

        Math::Vector pos1 = body1->getKinematicProperty("Position");
        Math::Vector pos2 = body2->getKinematicProperty("Position");

        double mass1 = body1->getPhysicalProperty("Mass");
        double mass2 = body2->getPhysicalProperty("Mass");

        // Calculate the Direction of the Force
        Math::Vector direction = pos2 - pos1;

        // Get the lenght of the direction(Distance between the centers of the bodies)
        double r = Math::Operation::Length(pos2 - pos1);


        // Normalise the direction
        direction = Math::Operation::Normalize(direction);

        // Calculate the Magnitude of the Force
        double forceMagnitude = G * (mass1 * mass2) / (r * r);

        // Calculate the Force
        Math::Vector force = direction * forceMagnitude;

        // Apply the force to the body
        body2->addKinematicProperty("GForce", -force);
        body1->addKinematicProperty("GForce", force);
    }

} // namespace Physics