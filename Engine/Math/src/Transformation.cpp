#include <cmath>

#include "../headers/Transformation.hpp"
#include "../headers/Vector.hpp"

using std::sin;
using std::cos;

// Define Zero transformation

namespace Math {

    const Math::Transformation Math::Transformation::Zero = Transformation(0, 0, 0);



    Transformation::Transformation(const Vector& position, float angle) :
        PositionX(position.x), PositionY(position.y),
        sine(std::sin(angle)), cosine(std::cos(angle)) {
    }

    Transformation::Transformation(float x, float y, float angle) :
        PositionX(x), PositionY(y),
        sine(std::sin(angle)), cosine(std::cos(angle)) {
    }

}