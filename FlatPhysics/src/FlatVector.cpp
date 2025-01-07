#include <string>


using std::string;
using std::to_string;

#include "../headers/FlatVector.hpp"
#include "../headers/FlatTransformation.hpp"

namespace FlatPhysics {
    // Define the zero vector
    FlatVector FlatVector::Zero = FlatVector(0, 0);
    // int FlatVector::copyConstructorCalls = 0;
    // int FlatVector::moveConstructorCalls = 0;


    FlatVector::FlatVector(float x, float y) : x(x), y(y) {}

    // Addition
    FlatVector FlatVector::operator+(const FlatVector& other) const {
        return FlatVector(x + other.x, y + other.y);
    }

    // Subtraction
    FlatVector FlatVector::operator-(const FlatVector& other) const {
        return FlatVector(x - other.x, y - other.y);
    }

    // Multiplication
    FlatVector FlatVector::operator*(float scalar) const {
        return FlatVector(x * scalar, y * scalar);
    }

    // Division
    FlatVector FlatVector::operator/(float scalar) const {
        return FlatVector(x / scalar, y / scalar);
    }

    // In-place addition
    FlatVector& FlatVector::operator+=(const FlatVector& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    // In-place subtraction
    FlatVector& FlatVector::operator-=(const FlatVector& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    // In-place multiplication
    FlatVector& FlatVector::operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    // In-place division
    FlatVector& FlatVector::operator/=(float scalar) {
        x /= scalar;
        y /= scalar;
        return *this;
    }


    FlatVector FlatVector::operator-() const {
        return FlatVector(-x, -y);
    }



    FlatVector operator*(float scalar, const FlatVector& vector) {
        return FlatVector(vector.x * scalar, vector.y * scalar);
    }



    bool FlatVector::operator==(const FlatVector& other) const {
        return x == other.x && y == other.y;
    }

    std::string FlatVector::toString() const {
        return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
    }

    FlatVector FlatVector::Transform(const FlatVector& vector, FlatTransformation& transform) {
        return FlatVector(
            vector.x * transform.cosine - vector.y * transform.sine + transform.PositionX,
            vector.x * transform.sine + vector.y * transform.cosine + transform.PositionY
        );
    }
} // namespace FlatPhysics