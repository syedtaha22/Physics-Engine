#define _USE_MATH_DEFINES
#include <cmath>

#include "../headers/FlatBody.hpp"
#include "../headers/FlatTransformation.hpp"
#include "../headers/FlatWorld.hpp"
#include "../headers/FlatMath.hpp"


FlatPhysics::FlatBody::FlatBody(FlatPhysics::FlatVector position, float density, float mass, float restitution, float area,
    bool isStatic, float radius, float width, float height, ShapeType shapeType) :
    position(position), density(density), mass(mass), restitution(restitution), area(area),
    isStatic(isStatic), radius(radius), width(width), height(height), shapeType(shapeType)
{

    this->velocity = FlatPhysics::FlatVector::Zero;
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

std::vector<int> FlatPhysics::FlatBody::createBoxTriangles() {
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
std::vector<FlatPhysics::FlatVector> FlatPhysics::FlatBody::createVertices(float& width, float& height) {
    float left = -width / 2;
    float right = left + width;
    float bottom = -height / 2;
    float top = bottom + height;

    std::vector<FlatPhysics::FlatVector> vertices(4);
    vertices[0] = FlatVector(left, top);
    vertices[1] = FlatVector(right, top);
    vertices[2] = FlatVector(right, bottom);
    vertices[3] = FlatVector(left, bottom);

    return vertices;
}

// Destructor
FlatPhysics::FlatBody::~FlatBody() {}

// Get Position
FlatPhysics::FlatVector FlatPhysics::FlatBody::getPosition() const {
    return position;
}


// User side functions to create a Circle body
bool FlatPhysics::FlatBody::createCircleBody(float radius, FlatPhysics::FlatVector position, float density, bool isStatic,
    float restitution, FlatBody*& body)
{
    float area = M_PI * radius * radius;

    if (area < FlatPhysics::FlatWorld::minBodySize || area > FlatPhysics::FlatWorld::maxBodySize) {
        return false;

    }

    if (density < FlatPhysics::FlatWorld::minDensity || density > FlatPhysics::FlatWorld::maxDensity) {
        return false;
    }

    restitution = FlatPhysics::FlatMath::Clamp(restitution, 0.0f, 1.0f);

    // Mass = density * area
    float mass = density * area;

    body = new FlatBody(position, density, mass, restitution, area, isStatic, radius, 0, 0, ShapeType::Circle);
    return true;
}

// User side functions to create a Box body
bool FlatPhysics::FlatBody::createBoxBody(float width, float height, FlatPhysics::FlatVector position, float density,
    bool isStatic, float restitution, FlatBody*& body)
{

    float area = width * height;

    if (area < FlatPhysics::FlatWorld::minBodySize || area > FlatPhysics::FlatWorld::maxBodySize) {
        return false;
    }



    if (density < FlatPhysics::FlatWorld::minDensity || density > FlatPhysics::FlatWorld::maxDensity) {
        return false;
    }

    restitution = FlatPhysics::FlatMath::Clamp(restitution, 0.0f, 1.0f);

    // Mass = density * area
    float mass = density * area;

    body = new FlatBody(position, density, mass, restitution, area, isStatic, 0, width, height, ShapeType::Box);
    return true;
}

void FlatPhysics::FlatBody::move(FlatPhysics::FlatVector amount) {

    position += amount;
    transformedUpdateRequired = true;
}

void FlatPhysics::FlatBody::moveTo(FlatPhysics::FlatVector newPosition) {
    position = newPosition;
    transformedUpdateRequired = true;
}

void FlatPhysics::FlatBody::rotate(float amount) {
    rotation += amount;
    transformedUpdateRequired = true;
}

void FlatPhysics::FlatBody::step(float time) {
    position += velocity * time;
    rotation += angularVelocity * time;
}

std::vector<FlatPhysics::FlatVector> FlatPhysics::FlatBody::getTransformedVertices() {
    if (transformedUpdateRequired) {
        // Make a transformation object
        FlatTransformation transformation(position, rotation);
        for (int i = 0; i < tranformedVertices.size(); i++) {
            // Check if vertices are not null
            if (vertices.size() > 0) {
                FlatVector v = vertices[i];
                tranformedVertices[i] = FlatVector::Transform(v, transformation);
            }
        }
    }
    transformedUpdateRequired = false;
    return tranformedVertices;
}





