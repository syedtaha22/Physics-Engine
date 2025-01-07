/*

    Represents a transformations

*/

#ifndef FLAT_TRANSFORMATION_HPP
#define FLAT_TRANSFORMATION_HPP

namespace FlatPhysics {

    // Forward Declaration of FlatVector
    struct FlatVector;

    struct FlatTransformation {
        const float PositionX;
        const float PositionY;
        const float sine;
        const float cosine;

        // Static Zero transformation
        static const FlatTransformation Zero;

        FlatTransformation(const FlatVector& position, float angle);

        FlatTransformation(float x, float y, float angle);
    };



} // namespace FlatPhysics




#endif // FLAT_TRANSFORMATION_HPP