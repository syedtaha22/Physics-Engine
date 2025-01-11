#include "../headers/BoundingBox.hpp"
#include "../headers/Vector.hpp"


namespace Math {

    BoundingBox::BoundingBox(Vector* min, Vector* max) : min(min), max(max) {}

    BoundingBox::BoundingBox(float minX, float minY, float maxX, float maxY) {
        min = new Vector(minX, minY);
        max = new Vector(maxX, maxY);
    }

    BoundingBox::~BoundingBox() {
        delete min;
        delete max;
    }


} // namespace Math