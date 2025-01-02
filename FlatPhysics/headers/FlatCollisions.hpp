/*

    Contains code for collision detection and resolution.

*/

#ifndef FLATCOLLISIONS_HPP
#define FLATCOLLISIONS_HPP

namespace FlatPhysics {

    // Forward Declaration of FlatVector
    struct FlatVector;

    class FlatCollisions {
    public:

        // Check if two circles are colliding
        static bool circleCircleCollision(const FlatVector& centerA, float radiusA,
            const FlatVector& centerB, float radiusB, FlatVector& normal, float& depth);
    };


} // namespace FlatPhysics


#endif // FLATCOLLISIONS_HPP