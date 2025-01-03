/*

    Body class for FlatPhysics library to represent a body in the physics engine.

*/

#ifndef FLATBODY_HPP
#define FLATBODY_HPP

#include <vector>

#include "FlatVector.hpp"

namespace FlatPhysics {

    enum class ShapeType {
        Circle = 0,
        Box = 1
    };

    class FlatBody {
        FlatVector position;
        FlatVector velocity;
        float rotation;
        float angularVelocity;

        std::vector<FlatVector> vertices;
        std::vector<FlatVector> tranformedVertices;

        bool transformedUpdateRequired;

    public:
        float const density;
        float const mass;
        float const restitution;
        float const area;

        bool const isStatic;

        float const radius;
        float const height;
        float const width;

        std::vector<int> triangles;
        ShapeType const shapeType;


    private:
        // Private constructor
        FlatBody(FlatVector position, float density, float mass, float restitution, float area,
            bool isStatic, float radius, float width, float height, ShapeType shapeType);

        std::vector<int> createBoxTriangles();

        // Function to get the vertices from the width and height
        std::vector<FlatVector> createVertices(float& width, float& height);

    public:

        // Destructor
        ~FlatBody();
        // Get Position
        FlatVector getPosition() const;


        // User side functions to create a Circle body
        static bool createCircleBody(float radius, FlatVector position, float density, bool isStatic,
            float restitution, FlatBody*& body);

        // User side functions to create a Box body
        static bool createBoxBody(float width, float height, FlatVector position, float density,
            bool isStatic, float restitution, FlatBody*& body);

        void move(FlatVector amount);

        void moveTo(FlatVector newPosition);

        void rotate(float amount);

        void step(float time);

        std::vector<FlatVector> getTransformedVertices();

    };
}



#endif // FLATBODY_HPP