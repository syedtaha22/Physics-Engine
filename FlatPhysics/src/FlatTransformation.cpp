#include <cmath>

#include "../headers/FlatTransformation.hpp"
#include "../headers/FlatVector.hpp"

using std::sin;
using std::cos;

// Define Zero transformation
const FlatPhysics::FlatTransformation FlatPhysics::FlatTransformation::Zero = FlatTransformation(0, 0, 0);

FlatPhysics::FlatTransformation::FlatTransformation(FlatVector position, float angle) :
    PositionX(position.x), PositionY(position.y),
    sine(std::sin(angle)), cosine(std::cos(angle)) {
}

FlatPhysics::FlatTransformation::FlatTransformation(float x, float y, float angle) :
    PositionX(x), PositionY(y),
    sine(std::sin(angle)), cosine(std::cos(angle)) {
}
