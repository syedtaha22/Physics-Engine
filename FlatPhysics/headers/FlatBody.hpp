/*

    Body class for FlatPhysics library to represent a body in the physics engine.

*/

#ifndef FLAT_BODY_HPP
#define FLAT_BODY_HPP

#include <vector>

#include "FlatVector.hpp"

namespace FlatPhysics {

    // Forward Declaration of FlatAABB
    struct FlatAABB;

    enum class ShapeType {
        Circle = 0,
        Box = 1
    };

    class FlatBody {
        FlatVector position;
        FlatVector force;

        float rotation;
        float angularVelocity;

        std::vector<FlatVector> vertices;
        std::vector<FlatVector> tranformedVertices;

        bool transformedUpdateRequired;

    public:
        ShapeType const shapeType;

        float const density;
        float const area;
        float const restitution;

        float const radius;
        float const height;
        float const width;

        bool const isStatic;

        float const mass;
        float const inverseMass;

        float const inertia;
        float const inverseInertia;

        FlatVector linearVelocity;
        std::vector<int> triangles;

    private:
        // Private constructor
        FlatBody(const FlatVector& position, float density, float mass, float restitution, float area,
            bool isStatic, float radius, float width, float height, ShapeType shapeType);

        std::vector<int> createBoxTriangles();

        // Function to get the vertices from the width and height
        std::vector<FlatVector> createVertices(float& width, float& height);

        float calculateInertia();

    public:

        // Destructor
        ~FlatBody();
        // Get Position
        const FlatVector& getPosition() const;


        // User side functions to create a Circle body
        static bool createCircleBody(float radius, const FlatVector& position, float density, bool isStatic,
            float restitution, FlatBody*& body);

        // User side functions to create a Box body
        static bool createBoxBody(float width, float height, const FlatVector& position, float density,
            bool isStatic, float restitution, FlatBody*& body);

        void move(const FlatVector& amount);

        void moveTo(const FlatVector& newPosition);

        void rotate(float amount);

        void step(float time, const FlatVector& gravity);

        void applyForce(const FlatVector& force);

        const std::vector<FlatVector>& getTransformedVertices();

        FlatAABB getAABB() const;
    };
}



#endif // FLAT_BODY_HPP