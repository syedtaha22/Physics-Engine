/*

    Body class for FlatPhysics library to represent a body in the physics engine.

*/

#ifndef FLATBODY_HPP
#define FLATBODY_HPP

// use math defines
#define _USE_MATH_DEFINES

#include "FlatVector.hpp"
#include "FlatWorld.hpp"
#include "FlatMath.hpp"
#include <cmath>

namespace FlatPhysics {

    enum class ShapeType {
        Circle = 0,
        Box = 1
    };

    class FlatBody {
        FlatPhysics::FlatVector position;
        FlatPhysics::FlatVector velocity;
        float rotation;
        float angularVelocity;

    public:
        float const density;
        float const mass;
        float const restitution;
        float const area;

        bool const isStatic;

        float const radius;
        float const height;
        float const width;

        ShapeType const shapeType;


    private:
        // Private constructor
        FlatBody(FlatPhysics::FlatVector position, float density, float mass, float restitution, float area,
            bool isStatic, float radius, float width, float height, ShapeType shapeType) :
            position(position), density(density), mass(mass), restitution(restitution), area(area),
            isStatic(isStatic), radius(radius), width(width), height(height), shapeType(shapeType) {

            this->velocity = FlatPhysics::FlatVector::Zero;
            this->rotation = 0;
            this->angularVelocity = 0;
        }

    public:

        // Get Position
        FlatPhysics::FlatVector getPosition() const {
            return position;
        }



        // User side functions to create a Circle body
        static bool createCircleBody(float radius, FlatPhysics::FlatVector position, float density, bool isStatic,
            float restitution, FlatBody*& body)
        {
            float area = M_PI * radius * radius;

            if (area < FlatPhysics::FlatWorld::minBodySize || area > FlatPhysics::FlatWorld::maxBodySize) {
                return false;

            }

            if (density < FlatPhysics::FlatWorld::minDensity || density > FlatPhysics::FlatWorld::maxDensity) {
                return false;
            }

            restitution = FlatPhysics::FlatMath::clamp(restitution, 0.0f, 1.0f);

            // Mass = density * area
            float mass = density * area;

            body = new FlatBody(position, density, mass, restitution, area, isStatic, radius, 0, 0, ShapeType::Circle);
            return true;
        }

        // User side functions to create a Box body
        static bool createBoxBody(float width, float height, FlatPhysics::FlatVector position, float density,
            bool isStatic, float restitution, FlatBody*& body)
        {

            float area = width * height;

            if (area < FlatPhysics::FlatWorld::minBodySize || area > FlatPhysics::FlatWorld::maxBodySize) {
                return false;
            }

            if (density < FlatPhysics::FlatWorld::minDensity || density > FlatPhysics::FlatWorld::maxDensity) {
                return false;
            }

            restitution = FlatPhysics::FlatMath::clamp(restitution, 0.0f, 1.0f);

            // Mass = density * area
            float mass = density * area;

            body = new FlatBody(position, density, mass, restitution, area, isStatic, 0, width, height, ShapeType::Box);
            return true;
        }

        void move(FlatPhysics::FlatVector amount) {
            position += amount;
        }

        void moveTo(FlatPhysics::FlatVector newPosition) {
            position = newPosition;
        }

    };
}



#endif // FLATBODY_HPP