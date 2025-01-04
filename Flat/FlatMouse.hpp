/*

    Contains Functions to see if a mouse button is pressed or released

*/

#ifndef FLATMOUSE_HPP
#define FLATMOUSE_HPP

#include <SFML/Window.hpp>

#include "FlatWindow.hpp"

namespace Flat {

    // Define Mouse Buttons
    enum class MouseButton {
        Left = sf::Mouse::Left,
        Right = sf::Mouse::Right,
        Middle = sf::Mouse::Middle,
    };

    class Mouse {
    public:
        static bool isLeftButtonClicked() {
            return sf::Mouse::isButtonPressed(sf::Mouse::Left);
        }

        static bool isRightButtonClicked() {
            return sf::Mouse::isButtonPressed(sf::Mouse::Right);
        }

        static bool isMiddleButtonClicked() {
            return sf::Mouse::isButtonPressed(sf::Mouse::Middle);
        }

        static bool isMouseButtonClicked(Window& window) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::MouseButtonPressed)
                    return true;
            }
            return false;
        }

        static sf::Vector2f getPosition(Flat::Window& window) {
            // Get the mouse position in the window (in pixels)
            sf::Vector2i mousePixelPos = sf::Mouse::getPosition(window.window);

            // Convert the pixel position to world coordinates using the camera's view
            return window.window.mapPixelToCoords(mousePixelPos, window.camera);
        }
    };



} // namespace Flat












#endif // FLATMOUSE_HPP