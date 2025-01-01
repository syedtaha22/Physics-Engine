/*
    This file contains the definition of the FlatVector class withing the FlatPhysics namespace.

*/

#ifndef FLAT_VECTOR_HPP
#define FLAT_VECTOR_HPP

#include <string>


namespace FlatPhysics {

    struct FlatVector {
        float x, y;

        // Define zero vector
        static FlatVector Zero;

        FlatVector() : x(0), y(0) {}
        FlatVector(float x, float y) : x(x), y(y) {}

        // Addition
        FlatVector operator+(const FlatVector& other) const {
            return FlatVector(x + other.x, y + other.y);
        }

        FlatVector& operator+=(const FlatVector& other) {
            x += other.x;
            y += other.y;
            return *this;
        }

        // Subtraction
        FlatVector operator-(const FlatVector& other) const {
            return FlatVector(x - other.x, y - other.y);
        }

        // Multiplication
        FlatVector operator*(float scalar) const {
            return FlatVector(x * scalar, y * scalar);
        }

        // Division
        FlatVector operator/(float scalar) const {
            return FlatVector(x / scalar, y / scalar);
        }

        // Check for equality
        bool operator==(const FlatVector& other) const {
            return x == other.x && y == other.y;
        }

        // To string
        std::string toString() const {
            return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
        }


    };

    // Define zero vector
    FlatVector FlatVector::Zero = FlatVector(0, 0);

} // namespace FlatPhysics


#endif // FLAT_VECTOR_HPP