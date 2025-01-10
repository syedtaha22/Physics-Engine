/*

    Contains information about the collision between two bodies, and how to resolve it.

*/

#ifndef FLAT_CONTACT_RESOLVER_HPP
#define FLAT_CONTACT_RESOLVER_HPP

#include "FlatVector.hpp"

namespace FlatPhysics {

    // Forward Declaration of FlatBody
    class FlatBody;

    struct FlatContactResolver {
        FlatBody* bodyA;
        FlatBody* bodyB;
        float depth;
        FlatVector normal;

        int contactCount;
        float e; // Restitution

        FlatVector Ra[2];
        FlatVector Rb[2];
        FlatVector impulses[2];
        FlatVector contactList[2];

        FlatContactResolver(FlatBody* bodyA, FlatBody* bodyB, float depth, const FlatVector& normal,
            const FlatVector& contact1, const FlatVector& contact2, int contactCount);

        FlatContactResolver();

    };


} // namespace FlatPhysics

#endif // FLAT_CONTACT_RESOLVER_HPP