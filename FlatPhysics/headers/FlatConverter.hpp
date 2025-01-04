// Converstion between FlatVectors and sfml vectors

#ifndef FLATCONVERTER_HPP
#define FLATCONVERTER_HPP

#include <vector>
#include <SFML/Graphics.hpp>

#include "FlatVector.hpp"


namespace FlatPhysics {

    class FlatConverter {
    public:
        // Convert FlatVector to sf::Vector2f
        static inline sf::Vector2f toVector2f(const FlatVector& v) {
            return sf::Vector2f(v.x, v.y);
        }

        // Convert to sf::Vector2f array
        static inline void toVector2fArray(const std::vector<FlatVector>& vertices,
            std::vector<sf::Vector2f>& sfVertices)
        {
            sfVertices.resize(vertices.size());
            for (int i = 0; i < vertices.size(); i++) {
                sfVertices[i] = sf::Vector2f(vertices[i].x, vertices[i].y);
            }
        }

        static inline FlatVector toFlatVector(const sf::Vector2f& v) {
            return FlatVector(v.x, v.y);
        }

        static inline FlatVector toFlatVector(const sf::Vector2i& v) {
            return FlatVector(v.x, v.y);
        }

    };

}

#endif // FLATCONVERTER_HPP
