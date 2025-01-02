/*

    Contains all the math functions and classes used in the physics engine.

*/

#ifndef FLATMATH_HPP
#define FLATMATH_HPP

namespace FlatPhysics {

    // Forward Declaration of FlatVector
    struct FlatVector;

    class FlatMath {
    public:

        // Dot Product
        static float DotProduct(const FlatPhysics::FlatVector& v1, const FlatPhysics::FlatVector& v2);

        // Cross Product
        static float CrossProduct(const FlatPhysics::FlatVector& v1, const FlatPhysics::FlatVector& v2);

        // Lenght of a vector
        static float Length(const FlatPhysics::FlatVector& v);

        // Normalize a vector
        static FlatPhysics::FlatVector Normalize(const FlatPhysics::FlatVector& v);

        // Distance between two points
        static float Distance(const FlatPhysics::FlatVector& v1, const FlatPhysics::FlatVector& v2);

        // Clamp
        static float Clamp(float value, float min, float max);
    };
}

#endif // FLATMATH_HPP