/*

    Contains all the math functions and classes used in the physics engine.

*/

#ifndef FLATMATH_HPP
#define FLATMATH_HPP

#include <vector>

namespace FlatPhysics {

    // Forward Declaration of FlatVector
    struct FlatVector;

    class FlatMath {
    public:

        static constexpr float FloatMax = 3.402823466e+38F; // Maximum value of a float
        static constexpr float FloatMin = 1.175494351e-38F; // Minimum value of a float


        // Dot Product
        static float DotProduct(const FlatVector& v1, const FlatVector& v2);

        // Cross Product
        static float CrossProduct(const FlatVector& v1, const FlatVector& v2);

        // Lenght of a vector
        static float Length(const FlatVector& v);

        // Normalize a vector
        static FlatVector Normalize(const FlatVector& v);

        // Distance between two points
        static float Distance(const FlatVector& v1, const FlatVector& v2);

        // Clamp
        static float Clamp(float value, float min, float max);

        static FlatVector ArtimeticMean(const std::vector<FlatVector>& vertices);


    };
}

#endif // FLATMATH_HPP