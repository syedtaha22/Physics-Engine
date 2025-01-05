/*

    Represents an Axis-Aligned Bounding Box (AABB) in 2D space.

*/

#ifndef FLAT_AABB_HPP
#define FLAT_AABB_HPP

namespace FlatPhysics {

    // Forward Declaration of FlatVector
    struct FlatVector;

    struct FlatAABB {
        FlatVector* min;
        FlatVector* max;

        FlatAABB(FlatVector* min, FlatVector* max);

        FlatAABB(float minX, float minY, float maxX, float maxY);

        ~FlatAABB();


    };


} // namespace FlatPhysics
















#endif // FLAT_AABB_HPP