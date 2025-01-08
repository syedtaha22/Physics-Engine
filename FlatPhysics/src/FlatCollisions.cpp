#include <iostream>

using std::cout;
using std::endl;

#include "../headers/FlatCollisions.hpp"
#include "../headers/FlatVector.hpp"
#include "../headers/FlatMath.hpp"
#include "../headers/FlatBody.hpp"
#include "../headers/FlatAABB.hpp"

namespace FlatPhysics {

    float FlatCollisions::MaxA = 0.0f;
    float FlatCollisions::MaxB = 0.0f;
    float FlatCollisions::MinA = 0.0f;
    float FlatCollisions::MinB = 0.0f;
    float FlatCollisions::AxisDepth = 0.0f;


    FlatVector FlatCollisions::getClosestPointOnSegment(const FlatVector& start,
        const FlatVector& end, const FlatVector& point, float& squaredDistance)
    {

        FlatVector direction = end - start; // Get the direction of the segment
        FlatVector fromStart = point - start; // Get the vector from the start to the point

        // Calculate the distance
        float d = FlatMath::DotProduct(fromStart, direction) / FlatMath::DotProduct(direction, direction);
        d = FlatMath::Clamp(d, 0.0f, 1.0f); // Clamp the distance to the segment

        FlatVector closestPoint = start + direction * d; // Calculate the closest point

        // Calculate squared distance
        squaredDistance = FlatMath::SquaredDistance(point, closestPoint);

        return closestPoint; // Return the closest point
    }

    void FlatCollisions::findCollisionPoint(const FlatVector& circleCenter, float radius,
        const FlatVector& polygonCenter, const std::vector<FlatVector>& vertices, FlatVector& contactPoint)
    {
        float minDistance = FlatMath::FloatMax;
        float distance_squared = 0.0f;

        // Loop over the Vertices of the Polygon
        for (int i = 0; i < vertices.size(); i++) {
            // Get the start of the segment
            const FlatVector& start = vertices[i];
            // Get the end of the segment
            const FlatVector& end = vertices[(i + 1) % vertices.size()];

            // Get the closest point on the segment to the circle center
            FlatVector closestPoint = getClosestPointOnSegment(start, end, circleCenter, distance_squared);

            if (distance_squared < minDistance) {
                minDistance = distance_squared;
                contactPoint = closestPoint;
            }
        }
    }

    void FlatCollisions::findCollisionPoint(const FlatVector& centerA, float radiusA,
        const FlatVector& centerB, FlatVector& closestPoint)
    {
        /*
        Get the normalized direction from center A to center B
            direction = Normalize(centerB - centerA);
        Calculate the collision point
            collisionPoint = centerA + direction * radiusA;
        */

        // Just do it in one line
        closestPoint = centerA + FlatMath::Normalize(centerB - centerA) * radiusA;
    }

    void FlatCollisions::getCollisionPoints(FlatBody*& bodyA, FlatBody*& bodyB,
        FlatVector& contact1, FlatVector& contact2, int& contactCount)
    {
        // Initilize contact points
        contact1 = FlatVector::Zero;
        contact2 = FlatVector::Zero;
        // Initialize contact count
        contactCount = 0;

        ShapeType shapeA = bodyA->shapeType;
        ShapeType shapeB = bodyB->shapeType;

        if (shapeA == ShapeType::Circle) {
            if (shapeB == ShapeType::Circle) {
                // Circle to Circle Collision, Only one contact point
                findCollisionPoint(bodyA->getPosition(), bodyA->radius, bodyB->getPosition(), contact1);
                contactCount = 1;
            }
            else if (shapeB == ShapeType::Box) {
                findCollisionPoint(bodyA->getPosition(), bodyA->radius,
                    bodyB->getPosition(), bodyB->getTransformedVertices(), contact1);
                contactCount = 1;
            }
        }
        else if (shapeA == ShapeType::Box) {
            if (shapeB == ShapeType::Circle) {
                findCollisionPoint(bodyB->getPosition(), bodyB->radius,
                    bodyA->getPosition(), bodyA->getTransformedVertices(), contact1);
                contactCount = 1;
            }
            else if (shapeB == ShapeType::Box) {
                findCollisionPoints(bodyA->getTransformedVertices(), bodyB->getTransformedVertices(),
                    contact1, contact2, contactCount);
            }
        }
    }

    void FlatCollisions::findCollisionPoints(const std::vector<FlatVector>& verticesA,
        const std::vector<FlatVector>& verticesB, FlatVector& contact1, FlatVector& contact2,
        int& contactCount)
    {
        FlatVector closestPointOnSegment;

        float minDistance = FlatMath::FloatMax;

        float distance_squared = 0.0f;

        for (int i = 0; i < verticesA.size(); i++) {
            for (int j = 0; j < verticesB.size(); j++) {
                const FlatVector& segment_start = verticesB[j];
                const FlatVector& segment_end = verticesB[(j + 1) % verticesB.size()];

                closestPointOnSegment = getClosestPointOnSegment(segment_start,
                    segment_end, verticesA[i], distance_squared);

                if (FlatMath::NearlyEqual(distance_squared, minDistance, 0.0001f)) {
                    if (!FlatMath::NearlyEqual(closestPointOnSegment, contact1, 0.0001f)) {
                        contact2 = closestPointOnSegment;
                        contactCount = 2;
                    }
                }
                else if (distance_squared < minDistance) {
                    minDistance = distance_squared;
                    contact1 = closestPointOnSegment;
                    contactCount = 1;
                }
            }
        }

        for (int i = 0; i < verticesB.size();i++) {
            for (int j = 0; j < verticesA.size(); j++) {
                const FlatVector& segment_start = verticesA[j];
                const FlatVector& segment_end = verticesA[(j + 1) % verticesA.size()];

                closestPointOnSegment = getClosestPointOnSegment(segment_start,
                    segment_end, verticesB[i], distance_squared);

                if (FlatMath::NearlyEqual(distance_squared, minDistance, 0.0001f)) {
                    if (!FlatMath::NearlyEqual(closestPointOnSegment, contact1, 0.0001f)) {
                        contact2 = closestPointOnSegment;
                        contactCount = 2;
                    }
                }
                else if (distance_squared < minDistance) {
                    minDistance = distance_squared;
                    contact1 = closestPointOnSegment;
                    contactCount = 1;
                }
            }
        }
    }


    bool FlatCollisions::collides(FlatBody*& bodyA, FlatBody*& bodyB,
        FlatVector& normal, float& depth)
    {
        ShapeType shapeA = bodyA->shapeType;
        ShapeType shapeB = bodyB->shapeType;

        if (shapeA == ShapeType::Circle) {
            if (shapeB == ShapeType::Circle) {
                return FlatCollisions::circleCircleCollision(bodyA->getPosition(), bodyA->radius,
                    bodyB->getPosition(), bodyB->radius, normal, depth);
            }
            else if (shapeB == ShapeType::Box) {
                return FlatCollisions::circlePolygonCollision(bodyA->getPosition(), bodyA->radius,
                    bodyB->getPosition(), bodyB->getTransformedVertices(), normal, depth);
            }
        }
        else if (shapeA == ShapeType::Box) {
            if (shapeB == ShapeType::Circle) {
                bool result = FlatCollisions::circlePolygonCollision(bodyB->getPosition(), bodyB->radius,
                    bodyA->getPosition(), bodyA->getTransformedVertices(), normal, depth);
                normal = -normal; // Reverse the normal
                return result;
            }
            else if (shapeB == ShapeType::Box) {
                return FlatCollisions::polygonPolygonCollision(
                    bodyA->getPosition(), bodyA->getTransformedVertices(),
                    bodyB->getPosition(), bodyB->getTransformedVertices(),
                    normal, depth);
            }
        }

        return false;
    }


    void FlatCollisions::ProjectVertices(const std::vector<FlatVector>& vertices,
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

    void FlatCollisions::ProjectCircle(const FlatVector& center, float radius,
        const FlatVector& axis, float& min, float& max)
    {

        // Calculate the Displacement
        // Axis is already normalized, so we can use it as the direction.
        FlatVector displacement = axis * radius;

        // Project the min and max points of the circle onto the axis
        min = FlatMath::DotProduct(center - displacement, axis);
        max = FlatMath::DotProduct(center + displacement, axis);

        if (min > max) {
            std::cout << "Min is greater than max" << std::endl;
            float temp = min;
            min = max;
            max = temp;
        }
    }

    int FlatCollisions::getClosestVertexIndex(const FlatVector& point, const std::vector<FlatVector>& vertices)
    {
        float minDistance = FlatMath::FloatMax;
        int closestIndex = 0;

        for (int i = 0; i < vertices.size(); i++) {
            float distance = FlatMath::SquaredDistance(point, vertices[i]);
            if (distance < minDistance) {
                minDistance = distance;
                closestIndex = i;
            }
        }

        return closestIndex;
    }


    bool FlatCollisions::circleCircleCollision(const FlatVector& centerA, float radiusA,
        const FlatVector& centerB, float radiusB, FlatVector& normal, float& depth)
    {
        /*
        Initially, I was initializing the normal to zero:
            normal = FlatVector::Zero;

        But Taking a look in to how the Distance function works,
        It first makes a vector pointing from one point to the other, then returns the lenght of the vector.

        Later we recalculate this vector(from A to B), and normalize it to get the normal.

        So, I decided to initialize the normal to the vector pointing from A to B,
        And get the lenght of the vector to calculate the distance. This reduces the number of calculations.

        */

        // Calculate the vector pointing from center A to center B
        normal = centerB - centerA;
        depth = 0.0f; // Initialize the depth to zero

        // Calculate the distance of the vector
        float distance = FlatMath::Length(normal);

        // Calculate the sum of the radii
        float radii = radiusA + radiusB;

        // Check if the distance is greater than the sum of the radii, then the circles are not colliding
        if (distance >= radii) return false;

        // Normalize the vector to get the normal
        normal = FlatMath::Normalize(normal);

        // Calculate the depth
        depth = radii - distance;

        return true;
    }

    bool FlatCollisions::polygonPolygonCollision(
        const FlatVector& centerA, const std::vector<FlatVector>& verticesA,
        const FlatVector& centerB, const std::vector<FlatVector>& verticesB,
        FlatVector& normal, float& depth)
    {

        normal = FlatVector::Zero;
        depth = FlatMath::FloatMax;

        // Allocate memory for variables only once instead of in each loop
        // float minA, maxA, minB, maxB;
        FlatVector edge = FlatVector::Zero;
        FlatVector axis = FlatVector::Zero;
        // float axisDepth = 0.0f;

        // Loop over body A vertices
        for (int i = 0; i < verticesA.size(); i++) {

            // Get the edge from the current vertex to the next vertex        
            edge = FlatMath::EdgeBetween(verticesA[i], verticesA[(i + 1) % verticesA.size()]);

            // Get the axis perpendicular to the edge in normalised space
            axis = FlatMath::Normalize(FlatVector(-edge.y, edge.x));

            // Project the vertices of both bodies onto the axis
            ProjectVertices(verticesA, axis, MinA, MaxA);
            ProjectVertices(verticesB, axis, MinB, MaxB);

            // Check if the projections overlap
            if (MaxA <= MinB || MaxB <= MinA) return false;

            // Get the depth of the overlap
            AxisDepth = std::min(MaxB - MinA, MaxA - MinB);

            // Check if the depth is less than the current depth
            if (AxisDepth < depth) {
                depth = AxisDepth;
                normal = axis;
            }
        }

        // Loop over body B vertices
        for (int i = 0; i < verticesB.size(); i++) {
            // Get the edge from the current vertex to the next vertex
            edge = FlatMath::EdgeBetween(verticesB[i], verticesB[(i + 1) % verticesB.size()]);

            // Get the axis perpendicular to the edge in normalised space
            axis = FlatMath::Normalize(FlatVector(-edge.y, edge.x));

            // Project the vertices of both bodies onto the axis
            ProjectVertices(verticesA, axis, MinA, MaxA);
            ProjectVertices(verticesB, axis, MinB, MaxB);

            // Check if the projections overlap
            if (MaxA <= MinB || MaxB <= MinA) return false;

            // Get the depth of the overlap
            AxisDepth = std::min(MaxB - MinA, MaxA - MinB);

            // Check if the depth is less than the current depth
            if (AxisDepth < depth) {
                depth = AxisDepth;
                normal = axis;
            }
        }

        /*
            Check if the normal is pointing from Body A to Body B
            Meaning that the normal should be in the direction we
            want to move Body B, in order to resolve the collision.

            - Get a vector pointing from the center of Body A to the center of Body B
            - Check if the dot product of the vector and the normal is less than 0
            - Reverse the normal if the dot product is less than 0
        */

        // Get the edge from the center of Body A to the center of Body B
        edge = centerB - centerA;

        // If the dot product of the edge and the normal is less than 0, reverse the normal
        if (FlatMath::DotProduct(edge, normal) < 0) normal = -normal;

        return true; // Return true since the bodies are colliding
    }


    bool FlatCollisions::circlePolygonCollision(const FlatVector& circleCenter, float radius,
        const FlatVector& polygonCenter, const std::vector<FlatVector>& vertices, FlatVector& normal, float& depth)
    {
        normal = FlatVector::Zero;
        depth = FlatMath::FloatMax;

        // Allocate memory for variables only once instead of in each loop
        // float minA, maxA, minB, maxB;
        FlatVector axis = FlatVector::Zero;
        FlatVector edge = FlatVector::Zero;
        // float axisDepth = 0.0f;

        for (int i = 0; i < vertices.size(); i++) {
            // Get the edge from the current vertex to the next vertex        
            edge = FlatMath::EdgeBetween(vertices[i], vertices[(i + 1) % vertices.size()]);

            // Get the axis perpendicular to the edge in normalised space
            axis = FlatMath::Normalize(FlatVector(-edge.y, edge.x));

            // Project the vertices of the polygon onto the axis
            ProjectVertices(vertices, axis, MinA, MaxA);
            // Project the circle onto the axis
            ProjectCircle(circleCenter, radius, axis, MinB, MaxB);

            // Check if the projections overlap
            if (MaxA <= MinB || MaxB <= MinA) return false;

            // Get the depth of the overlap
            AxisDepth = std::min(MaxB - MinA, MaxA - MinB);

            // Check if the depth is less than the current depth
            if (AxisDepth < depth) {
                depth = AxisDepth;
                normal = axis;
            }
        }

        int closestVertex = getClosestVertexIndex(circleCenter, vertices);

        // Get the edge from the Circle Center to the closest vertex of the polygon, and normalize it
        axis = FlatMath::Normalize(vertices[closestVertex] - circleCenter);

        // Project the vertices of the polygon onto the axis
        ProjectVertices(vertices, axis, MinA, MaxA);
        // Project the circle onto the axis
        ProjectCircle(circleCenter, radius, axis, MinB, MaxB);

        // Check if the projections overlap
        if (MaxA <= MinB || MaxB <= MinA) return false;

        // Get the depth of the overlap
        AxisDepth = std::min(MaxB - MinA, MaxA - MinB);

        // Check if the depth is less than the current depth
        if (AxisDepth < depth) {
            depth = AxisDepth;
            normal = axis;
        }

        /*
            In this case the Circle is Body A and the Polygon is Body B.

            Check if the normal is pointing from the Circle to the Polygon
            Meaning that the normal should be in the direction we
            want to move the Polygon, in order to resolve the collision.


            - Get a vector pointing from the center of the Circle to the center of the Polygon
            - Check if the dot product of the vector and the normal is less than 0
            - Reverse the normal if the dot product is less than 0
        */

        // Get the edge from the center of the Circle to the center of the Polygon
        edge = polygonCenter - circleCenter;

        // Check if the normal is pointing from the Circle to the Polygon
        if (FlatMath::DotProduct(edge, normal) < 0) normal = -normal;

        return true; // Return true since the bodies are colliding
    }

    bool FlatCollisions::AABB_AABB_Collision(const FlatAABB& a, const FlatAABB& b)
    {
        if (a.max->x <= b.min->x || b.max->x <= a.min->x) return false;
        if (a.max->y <= b.min->y || b.max->y <= a.min->y) return false;

        return true;
    }


} // namespace FlatPhysics