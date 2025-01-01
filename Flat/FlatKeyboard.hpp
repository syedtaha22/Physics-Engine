/*

    Contains functions to see if a key is pressed or released

*/

#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

#include <SFML/Window.hpp>

#include "FlatWindow.hpp"

namespace Flat {

    // Define Keys
    enum class Key {
        A = sf::Keyboard::A,
        B = sf::Keyboard::B,
        C = sf::Keyboard::C,
        D = sf::Keyboard::D,
        E = sf::Keyboard::E,
        F = sf::Keyboard::F,
        G = sf::Keyboard::G,
        H = sf::Keyboard::H,
        I = sf::Keyboard::I,
        J = sf::Keyboard::J,
        K = sf::Keyboard::K,
        L = sf::Keyboard::L,
        M = sf::Keyboard::M,
        N = sf::Keyboard::N,
        O = sf::Keyboard::O,
        P = sf::Keyboard::P,
        Q = sf::Keyboard::Q,
        R = sf::Keyboard::R,
        S = sf::Keyboard::S,
        T = sf::Keyboard::T,
        U = sf::Keyboard::U,
        V = sf::Keyboard::V,
        W = sf::Keyboard::W,
        X = sf::Keyboard::X,
        Y = sf::Keyboard::Y,
        Z = sf::Keyboard::Z,
        Num0 = sf::Keyboard::Num0,
        Num1 = sf::Keyboard::Num1,
        Num2 = sf::Keyboard::Num2,
        Num3 = sf::Keyboard::Num3,
        Num4 = sf::Keyboard::Num4,
        Num5 = sf::Keyboard::Num5,
        Num6 = sf::Keyboard::Num6,
        Num7 = sf::Keyboard::Num7,
        Num8 = sf::Keyboard::Num8,
        Num9 = sf::Keyboard::Num9,

        Left = sf::Keyboard::Left,
        Right = sf::Keyboard::Right,
        Up = sf::Keyboard::Up,
        Down = sf::Keyboard::Down,

    };

    enum class Event {
        KeyPressed = sf::Event::KeyPressed,
        KeyReleased = sf::Event::KeyReleased,
        MouseWheelScrolled = sf::Event::MouseWheelScrolled
    };

    class Keyboard {
    public:


        // Check if a key is pressed
        static bool isKeyPressed(Key key) {
            return sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(key));
        }

        // Check if a key is released
        static bool isKeyReleased(Key key) {
            return !sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(key));
        }

        static bool isKeyPressed(Flat::Window& window) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::KeyPressed) {
                    return true;
                }
            }
            return false;
        }

        // Function to see is any key is pressed


    };
} // namespace Flat

#endif // KEYBOARD_HPP