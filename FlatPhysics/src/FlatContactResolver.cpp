
#include "../headers/FlatContactResolver.hpp"

#include "../headers/FlatBody.hpp"

namespace FlatPhysics {


    FlatContactResolver::FlatContactResolver(FlatBody* bodyA, FlatBody* bodyB, float depth, const FlatVector& normal,
        const FlatVector& contact1, const FlatVector& contact2, int contactCount) :
        bodyA(bodyA), bodyB(bodyB), depth(depth), normal(normal), contactCount(contactCount)
    {
        contactList[0] = contact1;
        contactList[1] = contact2;
    }

    FlatContactResolver::FlatContactResolver() : bodyA(nullptr), bodyB(nullptr), depth(0),
        normal(FlatVector::Zero), contactCount(0) {
    }



} // namespace FlatPhysics
