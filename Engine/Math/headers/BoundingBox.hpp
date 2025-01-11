#ifndef BOUNDING_BOX_HPP
#define BOUNDING_BOX_HPP

namespace Math {

    // Forward Declaration of Vector
    struct Vector;

    // Axis Aligned Bounding Box
    struct BoundingBox {
        Vector* min;
        Vector* max;

        BoundingBox(Vector* min, Vector* max);

        BoundingBox(float minX, float minY, float maxX, float maxY);

        ~BoundingBox();

    };


} // namespace Math


#endif // BOUNDING_BOX_HPP