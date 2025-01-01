// Converstion between FlatVectors and sfml vectors

#ifndef FLATCONVERTER_HPP
#define FLATCONVERTER_HPP

#include <SFML/Graphics.hpp>
#include "FlatVector.hpp"

namespace FlatPhysics {

    class FlatConverter {
    public:
        // Convert FlatVector to sf::Vector2f
        static inline sf::Vector2f toSFML(const FlatPhysics::FlatVector& v) {
            return sf::Vector2f(v.x, v.y);
        }

        // Convert sf::Vector2f to FlatVector
        static inline FlatPhysics::FlatVector toFlat(const sf::Vector2f& v) {
            return FlatPhysics::FlatVector(v.x, v.y);
        }
    };

}

#endif // FLATCONVERTER_HPP
