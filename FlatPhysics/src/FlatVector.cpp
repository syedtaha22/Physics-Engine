#include <string>

using std::string;
using std::to_string;

#include "../headers/FlatVector.hpp"
#include "../headers/FlatTransformation.hpp"

// Define the zero vector
FlatPhysics::FlatVector FlatPhysics::FlatVector::Zero = FlatVector(0, 0);


FlatPhysics::FlatVector::FlatVector(float x, float y) : x(x), y(y) {}

FlatPhysics::FlatVector FlatPhysics::FlatVector::operator+(const FlatVector& other) const {
    return FlatVector(x + other.x, y + other.y);
}

FlatPhysics::FlatVector& FlatPhysics::FlatVector::operator+=(const FlatVector& other) {
    x += other.x;
    y += other.y;
    return *this;
}

FlatPhysics::FlatVector FlatPhysics::FlatVector::operator-(const FlatVector& other) const {
    return FlatVector(x - other.x, y - other.y);
}

FlatPhysics::FlatVector FlatPhysics::FlatVector::operator-() const {
    return FlatVector(-x, -y);
}

FlatPhysics::FlatVector FlatPhysics::FlatVector::operator*(float scalar) const {
    return FlatVector(x * scalar, y * scalar);
}

FlatPhysics::FlatVector FlatPhysics::FlatVector::operator/(float scalar) const {
    return FlatVector(x / scalar, y / scalar);
}

bool FlatPhysics::FlatVector::operator==(const FlatVector& other) const {
    return x == other.x && y == other.y;
}

std::string FlatPhysics::FlatVector::toString() const {
    return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
}

FlatPhysics::FlatVector FlatPhysics::FlatVector::Transform(const FlatVector& vector, FlatTransformation& transform) {
    return FlatVector(
        vector.x * transform.cosine - vector.y * transform.sine + transform.PositionX,
        vector.x * transform.sine + vector.y * transform.cosine + transform.PositionY
    );
}