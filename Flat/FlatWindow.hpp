#ifndef FLAT_WINDOW_HPP
#define FLAT_WINDOW_HPP

#include <iostream>
#include <SFML/Graphics.hpp>
#include "FlatShapes.hpp"
#include "FlatColor.hpp"

namespace Flat {

    enum class TimeUnit { Milliseconds, Seconds };

    class Window {
    private:
        sf::RenderWindow window;
        sf::View camera;
        sf::Clock clock;

        Flat::Color bgColor;
        float zoomLevel;

        // Store last frame time
        float lastFrameTime = 0.0f;

        static constexpr float pixelPerMeter = 100.0f; // 100 pixels per meter (adjust based on zoom level)

    public:


        // Window constructor with optional anti-aliasing parameter
        Window(unsigned int width, unsigned int height, const std::string& title, Flat::Color bgcolor = Flat::Color::Black,
            float zoom = 1.0f, int antiAliasingLevel = 8) : bgColor(bgcolor), zoomLevel(zoom) {
            sf::ContextSettings settings;
            settings.antialiasingLevel = antiAliasingLevel;  // Set anti-aliasing level

            // Create the window with anti-aliasing context settings
            window.create(sf::VideoMode(width, height), title, sf::Style::Default, settings);

            camera = sf::View(sf::FloatRect(
                -width / (2.0f * pixelPerMeter),
                -height / (2.0f * pixelPerMeter),
                width / pixelPerMeter,
                height / pixelPerMeter
            ));


            //camera = sf::View(sf::FloatRect(-10.0f, -10.0f, 10.0f, 10.0f));

            camera.setCenter(0.0f, 0.0f); // Center the camera explicitly at the origin (0,0)
            camera.zoom(zoomLevel); // Set the zoom level


            std::cout << "Zoom Level: " << zoomLevel << std::endl;

            window.setFramerateLimit(60);
            window.clear(bgColor.toSFML());
            window.setView(camera);
        }

        // Function to poll event
        bool pollEvent(sf::Event& event) {
            return window.pollEvent(event);
        }

        // Check if any key is pressed
        bool isKeyPressed() {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::KeyPressed) {
                    return true;
                }
            }
            return false;
        }

        // See if window is open
        bool isOpen() {
            return window.isOpen();
        }

        // Close the window
        void close() {
            window.close();
        }

        void clear() {
            window.clear(bgColor.toSFML());
        }

        void clear(Flat::Color color) {
            window.clear(color.toSFML());
        }

        // Function to change the window background color
        void setWindowColor(Flat::Color color) {
            bgColor = color;
            window.clear(bgColor.toSFML());

        }

        void display() {
            window.display();
        }

        // Unified Draw Function
        void draw(Shape& shape) {
            window.draw(shape.getShape());
        }

        void drawCircleWithBorder(float radius, sf::Vector2f position, Flat::Color borderColor = Flat::Color::Transparent,
            float borderThickness = 0, Flat::Color fillColor = Flat::Color::Transparent)
        {

            Flat::Circle circle(radius, position.x, position.y, borderColor, borderThickness, fillColor);
            draw(circle);
        }

        void drawCircleFilled(float radius, sf::Vector2f position, Flat::Color fillColor)
        {

            Flat::Circle circle(radius, position.x, position.y, Flat::Color::Transparent, 0, fillColor);
            draw(circle);
        }

        void drawRectangleWithBorder(float width, float height, sf::Vector2f position, Flat::Color borderColor = Flat::Color::Transparent,
            float borderThickness = 0, Flat::Color fillColor = Flat::Color::Transparent)
        {
            Flat::Rectangle rectangle(width, height, position.x, position.y, borderColor, borderThickness, fillColor);
            draw(rectangle);
        }

        void drawRectangleFilled(float width, float height, sf::Vector2f position, Flat::Color fillColor)
        {
            Flat::Rectangle rectangle(width, height, position.x, position.y, Flat::Color::Transparent, 0, fillColor);
            draw(rectangle);
        }

        void drawPolygonWithBorder(std::vector<sf::Vector2f> vertices, Flat::Color borderColor = Flat::Color::Transparent,
            float borderThickness = 0, Flat::Color fillColor = Flat::Color::Transparent)
        {
            Flat::Polygon polygon(vertices, borderColor, borderThickness, fillColor);
            draw(polygon);
        }

        void drawPolygonFilled(std::vector<sf::Vector2f> vertices, Flat::Color color) {
            Flat::Polygon polygon(vertices, Flat::Color::Transparent, 0, color);
            draw(polygon);
        }

        void drawLine(sf::Vector2f point1, sf::Vector2f point2, float thickness, Flat::Color color = Flat::Color::White) {
            Flat::Line line(point1, point2, thickness, color);
            draw(line);
        }

        void zoom(float factor) {
            zoomLevel *= factor;
            camera.zoom(factor);
            window.setView(camera);
        }

        void getCameraExtent(float& left, float& right, float& top, float& bottom) {
            sf::Vector2f size = camera.getSize();
            sf::Vector2f center = camera.getCenter();

            left = center.x - size.x / 2.0f;
            right = center.x + size.x / 2.0f;
            top = center.y - size.y / 2.0f;
            bottom = center.y + size.y / 2.0f;

        }

        float getElapsedTime(TimeUnit unit = TimeUnit::Seconds) {
            switch (unit) {
            case TimeUnit::Milliseconds:
                return clock.getElapsedTime().asMilliseconds();
            case TimeUnit::Seconds:
            default:
                return clock.getElapsedTime().asSeconds();
            }
        }

        float getElapsedTimeSinceLastFrame(TimeUnit unit = TimeUnit::Seconds) const {
            float currentTime = 0.0f;

            switch (unit) {
            case TimeUnit::Milliseconds:
                currentTime = clock.getElapsedTime().asMilliseconds();
                break;
            case TimeUnit::Seconds:
            default:
                currentTime = clock.getElapsedTime().asSeconds();
                break;
            }

            return currentTime - lastFrameTime; // Calculate delta time without updating lastFrameTime
        }

        void update() {
            lastFrameTime = clock.getElapsedTime().asSeconds(); // Update last frame time
        }

        ~Window() {
            if (window.isOpen()) {
                window.close();
            }
        }
    };

} // namespace Flat

#endif // FLAT_WINDOW_HPP
