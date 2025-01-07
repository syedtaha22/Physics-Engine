/*

    Contains code for collision detection and resolution.

*/

#ifndef FLATCOLLISIONS_HPP
#define FLATCOLLISIONS_HPP

#include <vector>

namespace FlatPhysics {

    // Forward Declaration of FlatVector, FlatBody and FlatAABB
    struct FlatVector;
    struct FlatAABB;
    class FlatBody;


    class FlatCollisions {
    private:
        // Declare these as static members, since they are being used in the functions
        // And I'd like to avoid memory allocated for them every time a function is called
        // Interestingly this gave a 10ms speedup in the time taken between steps
        static float MaxA;
        static float MinB;
        static float MaxB;
        static float MinA;
        static float AxisDepth;

    private:
        static void ProjectVertices(const std::vector<FlatVector>& vertices,
            const FlatVector& axis, float& min, float& max);

        static void ProjectCircle(const FlatVector& center, float radius,
            const FlatVector& axis, float& min, float& max);

        static int getClosestVertexIndex(const FlatVector& point, const std::vector<FlatVector>& vertices);

        // Returns the collision point of two circles
        static FlatVector getCollisionPoint(const FlatVector& centerA, float radiusA, const FlatVector& centerB);

        // Check if two circles are colliding
        static bool circleCircleCollision(const FlatVector& centerA, float radiusA,
            const FlatVector& centerB, float radiusB, FlatVector& normal, float& depth);


        // Check if two polygons are colliding
        static bool polygonPolygonCollision(
            const FlatVector& centerA, const std::vector<FlatVector>& verticesA,
            const FlatVector& centerB, const std::vector<FlatVector>& verticesB,
            FlatVector& normal, float& depth);


        // Check a circle and a polygon are colliding
        static bool circlePolygonCollision(const FlatVector& circleCenter, float radius,
            const FlatVector& polygonCenter, const std::vector<FlatVector>& vertices,
            FlatVector& normal, float& depth);

    public:

        static bool collides(FlatBody*& bodyA, FlatBody*& bodyB, FlatVector& normal, float& depth);

        static void getCollisionPoints(FlatBody*& bodyA, FlatBody*& bodyB,
            FlatVector& contact1, FlatVector& contact2, int& contactCount);

        // Check Collisions between Axis Aligned Bounding Boxes
        static bool AABB_AABB_Collision(const FlatAABB& a, const FlatAABB& b);

    };



} // namespace FlatPhysics


#endif // FLATCOLLISIONS_HPP