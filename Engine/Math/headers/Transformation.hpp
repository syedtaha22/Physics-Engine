#ifndef TRANSFORMATION_HPP
#define TRANSFORMATION_HPP

namespace Math {

    // Forward Declaration of FlatVector
    struct Vector;

    struct Transformation {
        const float PositionX;
        const float PositionY;
        const float sine;
        const float cosine;

        // Static Zero transformation
        static const Transformation Zero;

        Transformation(const Vector& position, float angle);

        Transformation(float x, float y, float angle);
    };



} // namespace Math




#endif // TRANSFORMATION_HPP