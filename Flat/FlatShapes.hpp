#ifndef FLAT_SHAPES_HPP
#define FLAT_SHAPES_HPP

#include <SFML/Graphics.hpp>
#include "FlatColor.hpp"

namespace Flat {

    static constexpr float pixelPerMeter = 100.0f; // 100 pixels per meter (adjust based on zoom level)


    // Base Shape Struct
    struct Shape {
        // Border properties
        float borderThickness = 0;
        Flat::Color borderColor = Flat::Color::Transparent;

        // Virtual function to retrieve the shape, to be overridden by derived shapes
        virtual sf::Drawable& getShape() = 0;

        // Method to add border
        void addBorder(float thickness, Flat::Color color) {
            borderThickness = thickness;
            borderColor = color;
            applyBorder(); // Apply border to the shape
        }

        // Method to apply border properties to the shape
        virtual void applyBorder() = 0;

        virtual ~Shape() = default;
    };

    // Rectangle Shape
    struct Rectangle : public Shape {
        sf::RectangleShape shape;

        // Constructor now takes width, height, x, y directly
        Rectangle(float width, float height, float x, float y, Flat::Color borderColor = Flat::Color::Transparent,
            float borderThickness = 0, Flat::Color fillColor = Flat::Color::Transparent)
        {
            shape.setSize(sf::Vector2f(width, height));  // Set size using width and height
            shape.setPosition(x, y);  // Set position using x and y
            shape.setOrigin(width / 2, height / 2);  // Set origin to the center of the rectangle
            shape.setFillColor(fillColor.toSFML());

            // Set border properties
            this->borderThickness = borderThickness;
            this->borderColor = borderColor;


            applyBorder();  // Apply border during construction
        }

        sf::Drawable& getShape() override {
            return shape;
        }

        // Override applyBorder to apply border to the rectangle shape
        void applyBorder() override {
            shape.setOutlineThickness(borderThickness);
            shape.setOutlineColor(borderColor.toSFML());
        }
    };

    // Circle Shape
    struct Circle : public Shape {
        sf::CircleShape shape;

        // Constructor now takes radius, x, y directly
        Circle(float radius, float x, float y, Flat::Color borderColor = Flat::Color::Transparent,
            float borderThickness = 0, Flat::Color fillColor = Flat::Color::Transparent)
        {
            shape.setRadius(radius);
            shape.setOrigin(radius, radius); // Set the origin to the center of the circle
            shape.setPosition(x, y);  // Set position using x and y
            shape.setFillColor(fillColor.toSFML());

            // Set border properties
            this->borderThickness = borderThickness;
            this->borderColor = borderColor;


            applyBorder();  // Apply border during construction
        }

        sf::Drawable& getShape() override {
            return shape;
        }

        // Override applyBorder to apply border to the circle shape
        void applyBorder() override {
            shape.setOutlineThickness(borderThickness);
            shape.setOutlineColor(borderColor.toSFML());
        }
    };

    // Polygon Shape
    struct Polygon : public Shape {
        sf::ConvexShape shape;

        // Constructor now takes a list of points directly
        Polygon(const std::vector<sf::Vector2f>& points, Flat::Color borderColor = Flat::Color::Transparent,
            float borderThickness = 0, Flat::Color fillColor = Flat::Color::Transparent)
        {
            shape.setPointCount(points.size());
            for (size_t i = 0; i < points.size(); i++) {
                shape.setPoint(i, points[i]);
            }
            shape.setFillColor(fillColor.toSFML());

            // Set border properties
            this->borderThickness = borderThickness;
            this->borderColor = borderColor;

            applyBorder();  // Apply border during construction
        }

        sf::Drawable& getShape() override {
            return shape;
        }

        // Override applyBorder to apply border to the polygon shape
        void applyBorder() override {
            shape.setOutlineThickness(borderThickness);
            shape.setOutlineColor(borderColor.toSFML());
        }
    };

    // Line Shape
    struct Line : public Shape {
        sf::VertexArray shape;

        // Constructor now takes two points directly
        Line(const sf::Vector2f& p1, const sf::Vector2f& p2, float thickness, Flat::Color color) {
            shape.setPrimitiveType(sf::Lines);
            shape.resize(2);
            shape[0].position = p1;
            shape[1].position = p2;
            shape[0].color = color.toSFML();
            shape[1].color = color.toSFML();
            shape[0].color = color.toSFML();
            shape[1].color = color.toSFML();
            applyBorder();  // Apply border during construction
        }

        sf::Drawable& getShape() override {
            return shape;
        }

        // No need for border for lines
        void applyBorder() override {}
    };

} // namespace Flat

#endif // FLAT_SHAPES_HPP
