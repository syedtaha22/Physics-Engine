/*

    Contains all the random functions used in the physics engine.

*/

#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <random>
#include "Flat/FlatColor.hpp"


namespace Random {

    // Random Number Generator
    static std::mt19937 randomEngine(std::random_device{}());

    // Random Float
    static float randomFloat(float min, float max) {
        std::uniform_real_distribution<float> dist(min, max);
        return dist(randomEngine);
    }

    // Random Int
    static int randomInt(int min, int max) {
        std::uniform_int_distribution<int> dist(min, max);
        return dist(randomEngine);
    }

    // Random Bool
    static bool randomBool() {
        return randomInt(0, 1) == 1;
    }

    // Random Color
    static Flat::Color randomColor() {
        return Flat::Color(randomInt(0, 255), randomInt(0, 255), randomInt(0, 255));
    }
}

#endif // RANDOM_HPP