/*

    Contains information about the collision between two bodies, and how to resolve it.

*/

#ifndef FLAT_MANIFOLD_HPP
#define FLAT_MANIFOLD_HPP

#include "FlatVector.hpp"

namespace FlatPhysics {

    // Forward Declaration of FlatBody
    class FlatBody;

    struct FlatManifold {
        FlatBody* bodyA;
        FlatBody* bodyB;
        float depth;
        FlatVector normal;

        FlatVector contact1;
        FlatVector contact2;
        int contactCount;

        FlatManifold(FlatBody* bodyA, FlatBody* bodyB, float depth, FlatVector normal, FlatVector
            contact1, FlatVector contact2, int contactCount);


    };


} // namespace FlatPhysics








#endif // FLAT_MANIFOLD_HPP