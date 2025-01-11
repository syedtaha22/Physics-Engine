#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <iostream>
#include <string>

namespace Math {

    // Forward Declaration of Transformation
    struct Transformation;

    struct Vector {
        float x, y;

        // Define zero vector
        static Vector Zero;

        Vector(float x = 0.0f, float y = 0.0f);


        Vector operator+(const Vector& other) const; // Addition
        Vector operator-(const Vector& other) const; // Subtraction
        Vector operator*(float scalar) const; // Multiplication
        Vector operator/(float scalar) const; // Division

        friend Vector operator*(float scalar, const Vector& vector); // Multiplication

        Vector& operator+=(const Vector& other); // In-place addition
        Vector& operator-=(const Vector& other); // In-place subtraction
        Vector& operator*=(float scalar); // In-place multiplication
        Vector& operator/=(float scalar); // In-place division

        Vector operator-() const; // Negation
        bool operator==(const Vector& other) const; // Check for equality
        std::string toString() const; // To string

        // Static function to transform a vector
        static Vector Transform(const Vector& vector, Transformation& transform);


    };

} // namespace Math


#endif // VECTOR_HPP