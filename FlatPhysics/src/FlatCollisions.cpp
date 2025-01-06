#include <iostream>

using std::cout;
using std::endl;

#include "../headers/FlatCollisions.hpp"
#include "../headers/FlatVector.hpp"
#include "../headers/FlatMath.hpp"
#include "../headers/FlatBody.hpp"

namespace FlatPhysics {

    FlatVector FlatCollisions::getCollisionPoint(const FlatVector& centerA, float radiusA,
        const FlatVector& centerB)
    {
        /*
        Get the normalized direction from center A to center B
            direction = Normalize(centerB - centerA);
        Calculate the collision point
            collisionPoint = centerA + direction * radiusA;
        */

        // Just do it in one line
        return centerA + FlatMath::Normalize(centerB - centerA) * radiusA;
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
                contact1 = getCollisionPoint(bodyA->getPosition(), bodyA->radius, bodyB->getPosition());
                contactCount = 1;
            }
            else if (shapeB == ShapeType::Box) {
                // To be implemented
            }
        }
        else if (shapeA == ShapeType::Box) {
            if (shapeB == ShapeType::Circle) {
                // To be implemented
            }
            else if (shapeB == ShapeType::Box) {
                // To be implemented
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

        // Initialize normal and depth
        normal = FlatVector::Zero;
        depth = 0.0f;

        // Calculate the distance between the two centers
        float distance = FlatMath::Distance(centerA, centerB);

        // Calculate the sum of the radii
        float radii = radiusA + radiusB;

        // Check if the distance is greater than the sum of the radii, then the circles are not colliding
        if (distance >= radii) return false;

        // Calculate the normal
        normal = FlatMath::Normalize(centerB - centerA);

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
        float minA, maxA, minB, maxB;
        FlatVector edge = FlatVector::Zero;
        FlatVector axis = FlatVector::Zero;
        float axisDepth = 0.0f;

        // Loop over body A vertices
        for (int i = 0; i < verticesA.size(); i++) {

            // Get the edge from the current vertex to the next vertex        
            edge = FlatMath::EdgeBetween(verticesA[i], verticesA[(i + 1) % verticesA.size()]);

            // Get the axis perpendicular to the edge in normalised space
            axis = FlatMath::Normalize(FlatVector(-edge.y, edge.x));

            // Project the vertices of both bodies onto the axis
            ProjectVertices(verticesA, axis, minA, maxA);
            ProjectVertices(verticesB, axis, minB, maxB);

            // Check if the projections overlap
            if (maxA <= minB || maxB <= minA) return false;

            // Get the depth of the overlap
            axisDepth = std::min(maxB - minA, maxA - minB);

            // Check if the depth is less than the current depth
            if (axisDepth < depth) {
                depth = axisDepth;
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
            ProjectVertices(verticesA, axis, minA, maxA);
            ProjectVertices(verticesB, axis, minB, maxB);

            // Check if the projections overlap
            if (maxA <= minB || maxB <= minA) return false;

            // Get the depth of the overlap
            axisDepth = std::min(maxB - minA, maxA - minB);

            // Check if the depth is less than the current depth
            if (axisDepth < depth) {
                depth = axisDepth;
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
        float minA, maxA, minB, maxB;
        FlatVector axis = FlatVector::Zero;
        FlatVector edge = FlatVector::Zero;
        float axisDepth = 0.0f;

        for (int i = 0; i < vertices.size(); i++) {
            // Get the edge from the current vertex to the next vertex        
            edge = FlatMath::EdgeBetween(vertices[i], vertices[(i + 1) % vertices.size()]);

            // Get the axis perpendicular to the edge in normalised space
            axis = FlatMath::Normalize(FlatVector(-edge.y, edge.x));

            // Project the vertices of the polygon onto the axis
            ProjectVertices(vertices, axis, minA, maxA);

            // Project the circle onto the axis
            ProjectCircle(circleCenter, radius, axis, minB, maxB);

            // Check if the projections overlap
            if (maxA <= minB || maxB <= minA) return false;

            // Get the depth of the overlap
            axisDepth = std::min(maxB - minA, maxA - minB);

            // Check if the depth is less than the current depth
            if (axisDepth < depth) {
                depth = axisDepth;
                normal = axis;
            }
        }

        int closestVertex = getClosestVertexIndex(circleCenter, vertices);

        // Get the edge from the Circle Center to the closest vertex of the polygon, and normalize it
        axis = FlatMath::Normalize(vertices[closestVertex] - circleCenter);

        // Project the vertices of the polygon onto the axis
        ProjectVertices(vertices, axis, minA, maxA);
        // Project the circle onto the axis
        ProjectCircle(circleCenter, radius, axis, minB, maxB);

        // Check if the projections overlap
        if (maxA <= minB || maxB <= minA) return false;

        // Get the depth of the overlap
        axisDepth = std::min(maxB - minA, maxA - minB);

        // Check if the depth is less than the current depth
        if (axisDepth < depth) {
            depth = axisDepth;
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


} // namespace FlatPhysics