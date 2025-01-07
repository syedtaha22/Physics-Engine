
#include "../headers/FlatManifold.hpp"

namespace FlatPhysics {


    FlatManifold::FlatManifold(FlatBody* bodyA, FlatBody* bodyB, float depth, const FlatVector& normal,
        const FlatVector& contact1, const FlatVector& contact2, int contactCount) :
        bodyA(bodyA), bodyB(bodyB), depth(depth), normal(normal), contact1(contact1),
        contact2(contact2), contactCount(contactCount) {
    }

} // namespace FlatPhysics
