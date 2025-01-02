#include "../headers/FlatCollisions.hpp"
#include "../headers/FlatVector.hpp"
#include "../headers/FlatMath.hpp"

bool FlatPhysics::FlatCollisions::circleCircleCollision(const FlatVector& centerA, float radiusA,
    const FlatVector& centerB, float radiusB, FlatVector& normal, float& depth) {

    normal = FlatVector::Zero;
    depth = 0.0f;

    float distance = FlatMath::Distance(centerA, centerB);
    float radii = radiusA + radiusB;

    if (distance >= radii) {
        return false;
    }


    normal = centerB - centerA;
    depth = radii - distance;

    return true;
}
