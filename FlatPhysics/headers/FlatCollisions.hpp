/*

    Contains code for collision detection and resolution.

*/

#ifndef FLATCOLLISIONS_HPP
#define FLATCOLLISIONS_HPP

#include <vector>

namespace FlatPhysics {

    // Forward Declaration of FlatVector and FlatBody
    struct FlatVector;
    class FlatBody;


    class FlatCollisions {
    private:
        static void ProjectVertices(const std::vector<FlatVector>& vertices,
            const FlatVector& axis, float& min, float& max);

        static void ProjectCircle(const FlatVector& center, float radius,
            const FlatVector& axis, float& min, float& max);

        static int getClosestVertexIndex(const FlatVector& point, const std::vector<FlatVector>& vertices);

        // Returns the collision point of two circles
        static FlatVector getCollisionPoint(const FlatVector& centerA, float radiusA, const FlatVector& centerB);


    public:

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

        static bool collides(FlatBody*& bodyA, FlatBody*& bodyB, FlatVector& normal, float& depth);

        static void getCollisionPoints(FlatBody*& bodyA, FlatBody*& bodyB,
            FlatVector& contact1, FlatVector& contact2, int& contactCount);

    };


} // namespace FlatPhysics


#endif // FLATCOLLISIONS_HPP