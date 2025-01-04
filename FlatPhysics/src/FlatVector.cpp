#include <string>

using std::string;
using std::to_string;

#include "../headers/FlatVector.hpp"
#include "../headers/FlatTransformation.hpp"

namespace FlatPhysics {
    // Define the zero vector
    FlatVector FlatVector::Zero = FlatVector(0, 0);


    FlatVector::FlatVector(float x, float y) : x(x), y(y) {}

    FlatVector FlatVector::operator+(const FlatVector& other) const {
        return FlatVector(x + other.x, y + other.y);
    }

    FlatVector& FlatVector::operator+=(const FlatVector& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    FlatVector FlatVector::operator-(const FlatVector& other) const {
        return FlatVector(x - other.x, y - other.y);
    }

    FlatVector& FlatVector::operator-=(const FlatVector& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }


    FlatVector FlatVector::operator-() const {
        return FlatVector(-x, -y);
    }

    FlatVector FlatVector::operator*(float scalar) const {
        return FlatVector(x * scalar, y * scalar);
    }


    FlatVector operator*(float scalar, const FlatVector& vector) {
        return FlatVector(vector.x * scalar, vector.y * scalar);
    }


    FlatVector FlatVector::operator/(float scalar) const {
        return FlatVector(x / scalar, y / scalar);
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