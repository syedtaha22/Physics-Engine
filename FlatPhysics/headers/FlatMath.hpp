/*

    Contains all the math functions and classes used in the physics engine.

*/

#ifndef FLATMATH_HPP
#define FLATMATH_HPP

#include <vector>
#include <cmath>
#include <string>
#include <sstream>
#include <iomanip>
#include "FlatVector.hpp"

namespace FlatPhysics {

    // Forward Declaration of FlatVector
    // struct FlatVector;

    class FlatMath {

    public:

        static constexpr float FloatMax = 3.4028235e+38F; // Maximum value of a float
        static constexpr float FloatMin = -3.4028235e+38F; // Minimum value of a float

        // Dot Product
        static inline float DotProduct(const FlatVector& v1, const FlatVector& v2) {
            return v1.x * v2.x + v1.y * v2.y;
        }

        // Cross Product
        static inline float CrossProduct(const FlatVector& v1, const FlatVector& v2) {
            return v1.x * v2.y - v1.y * v2.x;
        }

        // Lenght of a vector
        static inline float Length(const FlatVector& v) {
            return sqrt(v.x * v.x + v.y * v.y);
        }

        // Normalize a vector
        static inline FlatVector Normalize(const FlatVector& v) {
            float len = Length(v);
            return FlatVector(v.x / len, v.y / len);
        }

        // Distance between two points
        static inline float Distance(const FlatVector& v1, const FlatVector& v2) {
            return Length(v1 - v2);
        }

        // Squared Distance between two points
        static inline float SquaredDistance(const FlatVector& v1, const FlatVector& v2) {
            return (v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y);
        }

        // Clamp
        template <typename T>
        static T Clamp(T value, T min, T max) {
            if (value < min) return min;
            if (value > max) return max;
            return value;
        }

        static FlatVector ArtimeticMean(const std::vector<FlatVector>& vertices);

        // Edge between two points
        static inline FlatVector EdgeBetween(const FlatVector& p1, const FlatVector& p2) {
            /*
                Edge from point 1 to point 2:
                Edge = Point 2 - Point 1
            */
            return p2 - p1;
        }

        // Round a number
        static inline float Round(float number, size_t decimalPlaces) {
            float factor = static_cast<float>(std::pow(10, decimalPlaces));
            return std::round(std::trunc(number * factor)) / factor;
        }

        // Round a number and return as a string
        static inline std::string RoundToString(double number, size_t decimalPlaces) {
            std::ostringstream oss;
            oss << std::fixed << std::setprecision(decimalPlaces) << number;
            return oss.str();
        }


        static inline bool NearlyEqual(float a, float b, float epsilon) {
            return std::abs(a - b) < epsilon;
        }

        // Check if two vectors are nearly equal
        static inline bool NearlyEqual(const FlatVector& v1, const FlatVector& v2, float epsilon) {
            return SquaredDistance(v1, v2) < epsilon * epsilon;
        }

    };
} // namespace FlatPhysics

#endif // FLATMATH_HPP