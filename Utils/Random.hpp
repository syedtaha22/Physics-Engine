/*

    Contains all the random functions used in the physics engine.

*/

#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <random>
#include "../Flat/FlatColor.hpp"

namespace Utils {
    class Random {
    public:
        // Random Number Generator
        static std::mt19937 randomEngine;

        // Random Float
        static float Float(float min, float max) {
            std::uniform_real_distribution<float> dist(min, max);
            return dist(randomEngine);
        }

        // Random Int
        static int Int(int min, int max) {
            std::uniform_int_distribution<int> dist(min, max);
            return dist(randomEngine);
        }

        // Random Bool
        static bool Bool() {
            return Int(0, 1) == 1;
        }

        // Random Color
        static Flat::Color Color() {
            return Flat::Color(Int(0, 255), Int(0, 255), Int(0, 255));
        }
    };

    std::mt19937 Random::randomEngine(std::random_device{}());

} // namespace Utils

#endif // RANDOM_HPP