/*
    This file contains the definition of the FlatVector class withing the FlatPhysics namespace.

*/

#ifndef FLAT_VECTOR_HPP
#define FLAT_VECTOR_HPP

#include <string>

namespace FlatPhysics {

    // Forward Declaration of FlatTransformation
    struct FlatTransformation;

    struct FlatVector {
        float x, y;

        // Define zero vector
        static FlatVector Zero;

        FlatVector(float x = 0.0f, float y = 0.0f);

        FlatVector operator+(const FlatVector& other) const; // Addition
        FlatVector& operator+=(const FlatVector& other); // In-place addition
        FlatVector operator-(const FlatVector& other) const; // Subtraction
        FlatVector& operator-=(const FlatVector& other); // In-place subtraction
        FlatVector operator-() const; // Negation
        FlatVector operator*(float scalar) const; // Multiplication
        friend FlatVector operator*(float scalar, const FlatVector& vector); // Multiplication
        FlatVector operator/(float scalar) const; // Division
        bool operator==(const FlatVector& other) const; // Check for equality
        std::string toString() const; // To string
        static FlatVector Transform(const FlatVector& vector, FlatTransformation& transform); // Static function to transform a vector
    };

} // namespace FlatPhysics


#endif // FLAT_VECTOR_HPP