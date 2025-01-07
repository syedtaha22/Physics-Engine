#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <stdexcept>

#include "../headers/FlatBody.hpp"
#include "../headers/FlatTransformation.hpp"
#include "../headers/FlatWorld.hpp"
#include "../headers/FlatMath.hpp"
#include "../headers/FlatAABB.hpp"

namespace FlatPhysics {


    FlatBody::FlatBody(const FlatVector& position, float density, float mass, float restitution, float area,
        bool isStatic, float radius, float width, float height, ShapeType shapeType) :
        position(position), density(density), mass(mass), restitution(restitution), area(area),
        isStatic(isStatic), inverseMass(isStatic ? 0 : 1 / mass),
        radius(radius), width(width), height(height), shapeType(shapeType)
    {

        this->linearVelocity = FlatVector::Zero;
        this->force = FlatVector::Zero;
        this->rotation = 0;
        this->angularVelocity = 0;

        if (shapeType == ShapeType::Circle) {
            vertices = std::vector<FlatVector>();
            tranformedVertices = std::vector<FlatVector>();
            triangles = std::vector<int>();
        }
        else {
            vertices = createVertices(width, height);
            // Allocate memory for transformed vertices
            tranformedVertices = std::vector<FlatVector>(vertices.size());
            triangles = createBoxTriangles();
        }

        transformedUpdateRequired = true;
    }

    std::vector<int> FlatBody::createBoxTriangles() {
        std::vector<int> triangles(6);
        triangles[0] = 0;
        triangles[1] = 1;
        triangles[2] = 2;
        triangles[3] = 0;
        triangles[4] = 2;
        triangles[5] = 3;
        return triangles;

    }

    // Function to get the vertices from the width and height
    std::vector<FlatVector> FlatBody::createVertices(float& width, float& height) {
        float left = -width / 2;
        float right = left + width;
        float bottom = -height / 2;
        float top = bottom + height;

        std::vector<FlatVector> vertices(4);
        vertices[0] = FlatVector(left, top);
        vertices[1] = FlatVector(right, top);
        vertices[2] = FlatVector(right, bottom);
        vertices[3] = FlatVector(left, bottom);

        return vertices;
    }

    // Destructor
    FlatBody::~FlatBody() {}

    // Get Position
    const FlatVector& FlatBody::getPosition() const {
        return position;
    }


    // User side functions to create a Circle body
    bool FlatBody::createCircleBody(float radius, const FlatVector& position, float density, bool isStatic,
        float restitution, FlatBody*& body)
    {
        float area = M_PI * radius * radius;

        if (area < FlatWorld::MinBodySize || area > FlatWorld::MaxBodySize) {
            return false;

        }

        if (density < FlatWorld::MinDensity || density > FlatWorld::MaxDensity) {
            return false;
        }

        restitution = FlatMath::Clamp(restitution, 0.0f, 1.0f);

        // Mass = density * area
        float mass = density * area;

        body = new FlatBody(position, density, mass, restitution, area, isStatic, radius, 0, 0, ShapeType::Circle);
        return true;
    }

    // User side functions to create a Box body
    bool FlatBody::createBoxBody(float width, float height, const FlatVector& position, float density,
        bool isStatic, float restitution, FlatBody*& body)
    {

        float area = width * height;

        if (area < FlatWorld::MinBodySize || area > FlatWorld::MaxBodySize) {
            return false;
        }



        if (density < FlatWorld::MinDensity || density > FlatWorld::MaxDensity) {
            return false;
        }

        restitution = FlatMath::Clamp(restitution, 0.0f, 1.0f);

        // Mass = density * area
        float mass = density * area;

        body = new FlatBody(position, density, mass, restitution, area, isStatic, 0, width, height, ShapeType::Box);
        return true;
    }

    void FlatBody::move(const FlatVector& amount) {

        position += amount;
        transformedUpdateRequired = true;
    }

    void FlatBody::moveTo(const FlatVector& newPosition) {
        position = newPosition;
        transformedUpdateRequired = true;
    }

    void FlatBody::rotate(float amount) {
        rotation += amount;
        transformedUpdateRequired = true;
    }

    void FlatBody::step(float time, const FlatVector& gravity) {
        // Calculate acceleration
        //FlatVector acceleration = force * inverseMass;

        if (isStatic) return;

        // Update linear velocity
        linearVelocity += gravity * time;
        // Update position
        position += linearVelocity * time;

        // Update angular velocity
        rotation += angularVelocity * time;

        force = FlatVector::Zero;
        transformedUpdateRequired = true;
    }

    void FlatBody::applyForce(const FlatVector& force) {
        this->force = force;
    }

    const std::vector<FlatVector>& FlatBody::getTransformedVertices() {
        if (transformedUpdateRequired) {
            // Make a transformation object
            FlatTransformation transformation(position, rotation);
            for (int i = 0; i < tranformedVertices.size(); i++) {
                // Check if vertices are not null
                if (vertices.size() > 0) {
                    const FlatVector& v = vertices[i];
                    tranformedVertices[i] = FlatVector::Transform(v, transformation);
                }
            }
        }
        transformedUpdateRequired = false;
        return tranformedVertices;
    }

    FlatAABB FlatBody::getAABB() const {
        float minX = FlatMath::FloatMax;
        float minY = FlatMath::FloatMax;
        float maxX = FlatMath::FloatMin;
        float maxY = FlatMath::FloatMin;

        if (shapeType == ShapeType::Circle) {
            minX = position.x - radius;
            minY = position.y - radius;

            maxX = position.x + radius;
            maxY = position.y + radius;
        }
        else if (shapeType == ShapeType::Box) {
            for (int i = 0; i < tranformedVertices.size(); i++) {
                const FlatVector& v = tranformedVertices[i];
                if (v.x < minX) minX = v.x;
                if (v.y < minY) minY = v.y;

                if (v.x > maxX) maxX = v.x;
                if (v.y > maxY) maxY = v.y;
            }
        }
        else {
            throw std::runtime_error("Invalid Shape Type");
        }

        return FlatAABB(minX, minY, maxX, maxY);

    }

} // namespace FlatPhysics