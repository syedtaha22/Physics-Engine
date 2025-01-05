#include <cmath>

using std::sqrt;

#include "../headers/FlatMath.hpp"
#include "../headers/FlatVector.hpp"


namespace FlatPhysics {
    FlatVector FlatMath::ArtimeticMean(const std::vector<FlatVector>& vertices) {
        float sumX = 0.0f;
        float sumY = 0.0f;
        for (int i = 0; i < vertices.size(); i++) {
            sumX += vertices[i].x;
            sumY += vertices[i].y;
        }
        return FlatVector(sumX / vertices.size(), sumY / vertices.size());
    }
} // namespace FlatPhysics