

#include "../headers/FlatAABB.hpp"
#include "../headers/FlatVector.hpp"


namespace FlatPhysics {

    FlatAABB::FlatAABB(FlatVector* min, FlatVector* max) : min(min), max(max) {}

    FlatAABB::FlatAABB(float minX, float minY, float maxX, float maxY) {
        min = new FlatVector(minX, minY);
        max = new FlatVector(maxX, maxY);
    }

    FlatAABB::~FlatAABB() {
        delete min;
        delete max;
    }


} // namespace FlatPhysics