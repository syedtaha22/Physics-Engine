

#include "../headers/FlatCollisions.hpp"
#include "../headers/FlatVector.hpp"
#include "../headers/FlatMath.hpp"


void FlatPhysics::FlatCollisions::ProjectVertices(const std::vector<FlatVector>& vertices,
    const FlatVector& axis, float& min, float& max)
{
    min = FlatMath::DotProduct(vertices[0], axis);
    max = min;

    for (int i = 1; i < vertices.size(); i++) {
        float projection = FlatMath::DotProduct(vertices[i], axis);
        if (projection < min) {
            min = projection;
        }
        if (projection > max) {
            max = projection;
        }
    }
}

void FlatPhysics::FlatCollisions::ProjectCircle(const FlatVector& center, float radius,
    const FlatVector& axis, float& min, float& max)
{
    FlatVector displacement = FlatMath::Normalize(axis) * radius;

    min = FlatMath::DotProduct(center + displacement, axis);
    max = FlatMath::DotProduct(center - displacement, axis);

    if (min > max) {
        float temp = min;
        min = max;
        max = temp;
    }
}

int FlatPhysics::FlatCollisions::getClosestVertexIndex(const FlatVector& point, const std::vector<FlatVector>& vertices)
{
    float minDistance = FlatMath::FloatMax;
    int closestIndex = 0;

    for (int i = 0; i < vertices.size(); i++) {
        float distance = FlatMath::Distance(point, vertices[i]);
        if (distance < minDistance) {
            minDistance = distance;
            closestIndex = i;
        }
    }

    return closestIndex;
}


bool FlatPhysics::FlatCollisions::circleCircleCollision(const FlatVector& centerA, float radiusA,
    const FlatVector& centerB, float radiusB, FlatVector& normal, float& depth)
{

    normal = FlatVector::Zero;
    depth = 0.0f;

    float distance = FlatMath::Distance(centerA, centerB);
    float radii = radiusA + radiusB;

    if (distance >= radii) {
        return false;
    }
    normal = FlatMath::Normalize(centerB - centerA);

    depth = radii - distance;

    return true;
}

bool FlatPhysics::FlatCollisions::polygonPolygonCollision(
    const FlatVector& centerA, const std::vector<FlatVector>& verticesA,
    const FlatVector& centerB, const std::vector<FlatVector>& verticesB,
    FlatVector& normal, float& depth)
{

    normal = FlatVector::Zero;
    depth = FlatMath::FloatMax;

    // Loop over body A vertices
    for (int i = 0; i < verticesA.size(); i++) {
        FlatVector v1 = verticesA[i];
        FlatVector v2 = verticesA[(i + 1) % verticesA.size()];

        FlatVector edge = v2 - v1;
        FlatVector axis = FlatVector(-edge.y, edge.x);
        axis = FlatMath::Normalize(axis);

        float minA, maxA, minB, maxB;
        ProjectVertices(verticesA, axis, minA, maxA);
        ProjectVertices(verticesB, axis, minB, maxB);

        if (maxA <= minB || maxB <= minA) {
            return false;
        }

        float axisDepth = std::min(maxB - minA, maxA - minB);
        if (axisDepth < depth) {
            depth = axisDepth;
            normal = axis;
        }

    }

    // Loop over body B vertices
    for (int i = 0; i < verticesB.size(); i++) {
        FlatVector v1 = verticesB[i];
        FlatVector v2 = verticesB[(i + 1) % verticesB.size()];

        FlatVector edge = v2 - v1;
        FlatVector axis = FlatVector(-edge.y, edge.x);
        axis = FlatMath::Normalize(axis);

        float minA, maxA, minB, maxB;
        ProjectVertices(verticesA, axis, minA, maxA);
        ProjectVertices(verticesB, axis, minB, maxB);

        if (maxA <= minB || maxB <= minA) {
            return false;
        }

        float axisDepth = std::min(maxB - minA, maxA - minB);
        if (axisDepth < depth) {
            depth = axisDepth;
            normal = axis;
        }
    }

    FlatVector centerDifference = centerB - centerA;

    if (FlatMath::DotProduct(centerDifference, normal) < 0) {
        normal = -normal;
    }

    return true;
}


bool FlatPhysics::FlatCollisions::circlePolygonCollision(const FlatVector& circleCenter, float radius,
    const FlatVector& polygonCenter, const std::vector<FlatVector>& vertices, FlatVector& normal, float& depth)
{
    normal = FlatVector::Zero;
    depth = FlatMath::FloatMax;

    for (int i = 0; i < vertices.size(); i++) {
        FlatVector v1 = vertices[i];
        FlatVector v2 = vertices[(i + 1) % vertices.size()];

        FlatVector edge = v2 - v1;
        FlatVector axis = FlatVector(-edge.y, edge.x);
        axis = FlatMath::Normalize(axis);

        float minA, maxA, minB, maxB;
        ProjectVertices(vertices, axis, minA, maxA);
        ProjectCircle(circleCenter, radius, axis, minB, maxB);

        if (maxA <= minB || maxB <= minA) {
            return false;
        }

        float axisDepth = std::min(maxB - minA, maxA - minB);
        if (axisDepth < depth) {
            depth = axisDepth;
            normal = axis;
        }
    }

    int closestVertexIndex = getClosestVertexIndex(circleCenter, vertices);
    FlatVector closestVertex = vertices[closestVertexIndex];

    FlatVector axis = closestVertex - circleCenter;
    axis = FlatMath::Normalize(axis);

    float minA, maxA, minB, maxB;
    ProjectVertices(vertices, axis, minA, maxA);
    ProjectCircle(circleCenter, radius, axis, minB, maxB);

    if (maxA <= minB || maxB <= minA) {
        return false;
    }

    float axisDepth = std::min(maxB - minA, maxA - minB);
    if (axisDepth < depth) {
        depth = axisDepth;
        normal = axis;
    }

    FlatVector centerDifference = polygonCenter - circleCenter;

    if (FlatMath::DotProduct(centerDifference, normal) < 0) {
        normal = -normal;
    }

    return true;
}
