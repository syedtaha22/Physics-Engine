#include <cmath>

using std::sqrt;

#include "../headers/FlatMath.hpp"
#include "../headers/FlatVector.hpp"


float FlatPhysics::FlatMath::DotProduct(const FlatPhysics::FlatVector& v1, const FlatPhysics::FlatVector& v2) {
    return v1.x * v2.x + v1.y * v2.y;
}

float FlatPhysics::FlatMath::CrossProduct(const FlatPhysics::FlatVector& v1, const FlatPhysics::FlatVector& v2) {
    return v1.x * v2.y - v1.y * v2.x;
}

float FlatPhysics::FlatMath::Length(const FlatPhysics::FlatVector& v) {
    return std::sqrt(v.x * v.x + v.y * v.y);
}

FlatPhysics::FlatVector FlatPhysics::FlatMath::Normalize(const FlatPhysics::FlatVector& v) {
    float len = Length(v);
    return FlatPhysics::FlatVector(v.x / len, v.y / len);
}

float FlatPhysics::FlatMath::Distance(const FlatPhysics::FlatVector& v1, const FlatPhysics::FlatVector& v2) {
    return Length(v1 - v2);
}

float FlatPhysics::FlatMath::Clamp(float value, float min, float max) {
    return std::max(min, std::min(value, max));
}
