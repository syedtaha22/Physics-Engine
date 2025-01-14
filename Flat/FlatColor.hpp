#ifndef FLAT_COLOR_HPP
#define FLAT_COLOR_HPP

#include <SFML/Graphics/Color.hpp>
#include <stdexcept>
#include <ostream>

namespace Flat {

    struct Color {
        unsigned char r, g, b, a;

        // Default Constructor
        Color()
            : r(0), g(0), b(0), a(255) {
        }

        // Constructor for RGB(A)
        Color(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255)
            : r(red), g(green), b(blue), a(alpha) {
        }

        // Constructor for Hex color string "#RRGGBB" or "#RRGGBBAA"
        Color(const std::string& hex) {
            if (hex[0] == '#') {
                if (hex.length() == 7) {
                    // Format: #RRGGBB
                    r = std::stoi(hex.substr(1, 2), nullptr, 16);
                    g = std::stoi(hex.substr(3, 2), nullptr, 16);
                    b = std::stoi(hex.substr(5, 2), nullptr, 16);
                    a = 255; // Default to fully opaque
                }
                else if (hex.length() == 9) {
                    // Format: #RRGGBBAA
                    r = std::stoi(hex.substr(1, 2), nullptr, 16);
                    g = std::stoi(hex.substr(3, 2), nullptr, 16);
                    b = std::stoi(hex.substr(5, 2), nullptr, 16);
                    a = std::stoi(hex.substr(7, 2), nullptr, 16);
                }
                else {
                    throw std::invalid_argument("Invalid hex color format");
                }
            }
            else {
                throw std::invalid_argument("Hex color must start with '#' character");
            }
        }

        Color withAlha(unsigned char alpha) const {
            return Color(r, g, b, alpha);
        }

        // Predefined Colors
        static const Color Red;
        static const Color Green;
        static const Color Blue;
        static const Color White;
        static const Color Black;
        static const Color Yellow;
        static const Color Orange;
        static const Color Transparent;

        // Dark Colors
        static const Color DarkRed;
        static const Color DarkGreen;
        static const Color DarkBlue;


        // Conversion to SFML Color
        sf::Color toSFML() const {
            return sf::Color(r, g, b, a);
        }
    };

    // Predefined Color Definitions
    const Color Color::Red = Color(255, 0, 0);
    const Color Color::Green = Color(0, 255, 0);
    const Color Color::Blue = Color(0, 0, 255);
    const Color Color::White = Color(255, 255, 255);
    const Color Color::Black = Color(0, 0, 0);
    const Color Color::Yellow = Color(255, 255, 0);
    const Color Color::Orange = Color(255, 165, 0);
    const Color Color::Transparent = Color(0, 0, 0, 0);

    // Dark Colors
    const Color Color::DarkRed = Color(139, 0, 0);
    const Color Color::DarkGreen = Color(0, 100, 0);
    const Color Color::DarkBlue = Color(0, 0, 139);

    // Overloading the << operator for debugging
    inline std::ostream& operator<<(std::ostream& os, const Color& color) {
        os << "Color(R: " << static_cast<int>(color.r)
            << ", G: " << static_cast<int>(color.g)
            << ", B: " << static_cast<int>(color.b)
            << ", A: " << static_cast<int>(color.a) << ")";
        return os;
    }

}

#endif // FLAT_COLOR_HPP
