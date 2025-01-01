/*

    Contains all the math functions and classes used in the physics engine.

*/

#ifndef FLATMATH_HPP
#define FLATMATH_HPP

#include "FlatVector.hpp"
#include <cmath>

namespace FlatPhysics {

    class FlatMath {
    public:

        // Dot Product
        static float dotProduct(const FlatPhysics::FlatVector& v1, const FlatPhysics::FlatVector& v2) {
            return v1.x * v2.x + v1.y * v2.y;
        }

        // Cross Product
        static float crossProduct(const FlatPhysics::FlatVector& v1, const FlatPhysics::FlatVector& v2) {
            return v1.x * v2.y - v1.y * v2.x;
        }

        // Lenght of a vector
        static float length(const FlatPhysics::FlatVector& v) {
            return sqrt(v.x * v.x + v.y * v.y);
        }

        // Normalize a vector
        static FlatPhysics::FlatVector normalize(const FlatPhysics::FlatVector& v) {
            float len = length(v);
            return FlatPhysics::FlatVector(v.x / len, v.y / len);
        }

        // Distance between two points
        static float distance(const FlatPhysics::FlatVector& v1, const FlatPhysics::FlatVector& v2) {
            return length(v1 - v2);
        }

        // Clamp
        static float clamp(float value, float min, float max) {
            return std::max(min, std::min(value, max));
        }



    };



}







#endif // FLATMATH_HPP