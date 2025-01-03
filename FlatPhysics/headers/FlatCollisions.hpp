/*

    Contains code for collision detection and resolution.

*/

#ifndef FLATCOLLISIONS_HPP
#define FLATCOLLISIONS_HPP

#include <vector>

namespace FlatPhysics {

    // Forward Declaration of FlatVector
    struct FlatVector;

    class FlatCollisions {
    private:
        static void ProjectVertices(const std::vector<FlatVector>& vertices,
            const FlatVector& axis, float& min, float& max);

        static void ProjectCircle(const FlatVector& center, float radius,
            const FlatVector& axis, float& min, float& max);

        static int getClosestVertexIndex(const FlatVector& point, const std::vector<FlatVector>& vertices);


    public:

        // Check if two circles are colliding
        static bool circleCircleCollision(const FlatVector& centerA, float radiusA,
            const FlatVector& centerB, float radiusB, FlatVector& normal, float& depth);


        // Check if two polygons are colliding
        static bool polygonPolygonCollision(const std::vector<FlatVector>& verticesA,
            const std::vector<FlatVector>& verticesB, FlatVector& normal, float& depth);

        // Check a circle and a polygon are colliding
        static bool circlePolygonCollision(const FlatVector& circleCenter, float radius,
            const std::vector<FlatVector>& vertices, FlatVector& normal, float& depth);
    };


} // namespace FlatPhysics


#endif // FLATCOLLISIONS_HPP