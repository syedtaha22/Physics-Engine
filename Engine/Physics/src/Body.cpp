#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <stdexcept>


// Physics Headers
#include "../headers/Body.hpp"

// Math Headers
#include "../../Math/headers/Vector.hpp"


// #include "../headers/Transformation.hpp"
// #include "../headers/Vector.hpp"
// #include "../headers/FlatTransformation.hpp"
// #include "../headers/FlatWorld.hpp"
// #include "../headers/FlatMath.hpp"
// #include "../headers/FlatAABB.hpp"

namespace Physics {


    Body::Body(double radius, double width, double height, double mass, const Math::Vector& position)
    {

        // Add Kinematic Properties
        kinematicProperties["Position"] = position;
        kinematicProperties["LinearVelocity"] = Math::Vector(0, 0);
        kinematicProperties["Force"] = Math::Vector(0, 0);

        if (radius > 0) physicalProperties["Radius"] = radius;
        else if (width > 0 && height > 0) {
            physicalProperties["Width"] = width;
            physicalProperties["Height"] = height;
        }

        // Add Physical Properties
        physicalProperties["Mass"] = mass;
        physicalProperties["InverseMass"] = 1 / mass;
        physicalProperties["Restitution"] = 0.0f;

        // this->angle = 0;
        // this->angularVelocity = 0;

        // Set Boolean Properties
        booleanProperties["isStatic"] = false;
        booleanProperties["transformedUpdateRequired"] = true;
    }

    // Set Physical Property
    void Body::setPhysicalProperty(const std::string& property, double value) {
        physicalProperties[property] = value;
    }

    // Check if Physical Property exists
    bool Body::physicalPropertyExists(const std::string& property) const {
        return physicalProperties.find(property) != physicalProperties.end();
    }

    // Get Physical Property
    double Body::getPhysicalProperty(const std::string& property) const {
        if (physicalPropertyExists(property)) {
            return physicalProperties.at(property);
        }
        throw std::runtime_error("Property \"" + property + "\" does not exist");
    }

    // Set Kinematic Property
    void Body::setKinematicProperty(const std::string& property, const Math::Vector& value) {
        kinematicProperties[property] = value;
    }

    // Check if Kinematic Property exists
    bool Body::kinematicPropertyExists(const std::string& property) const {
        return kinematicProperties.find(property) != kinematicProperties.end();
    }

    // Get Kinematic Property
    Math::Vector Body::getKinematicProperty(const std::string& property) const {
        if (kinematicPropertyExists(property)) {
            return kinematicProperties.at(property);
        }
        throw std::runtime_error("Property \"" + property + "\" does not exist");
    }




    // std::vector<int> Body::createBoxTriangles() {
    //     std::vector<int> triangles(6);
    //     triangles[0] = 0;
    //     triangles[1] = 1;
    //     triangles[2] = 2;
    //     triangles[3] = 0;
    //     triangles[4] = 2;
    //     triangles[5] = 3;
    //     return triangles;

    // }

    // Function to get the vertices from the width and height
    // std::vector<FlatVector> Body::createVertices(float& width, float& height) {
    //     float left = -width / 2;
    //     float right = left + width;
    //     float bottom = -height / 2;
    //     float top = bottom + height;

    //     std::vector<FlatVector> vertices(4);
    //     vertices[0] = FlatVector(left, top);
    //     vertices[1] = FlatVector(right, top);
    //     vertices[2] = FlatVector(right, bottom);
    //     vertices[3] = FlatVector(left, bottom);

    //     return vertices;
    // }

    // // Destructor
    // Body::~Body() {}

    // // Get Position
    // const FlatVector& Body::getPosition() const {
    //     return position;
    // }

    // double Body::getAngle() const {
    //     return angle;
    // }

    std::shared_ptr<Body> Body::AstronomicalBody(double mass, const Math::Vector& position) {
        return std::make_shared<Body>(0, 0, 0, mass, position);
    }


    // User side functions to create a Circle body
    std::shared_ptr<Body> Body::CreateCircularBody(double radius, double mass, const Math::Vector& position)
    {
        if (radius <= 0.0f) throw std::runtime_error("Radius must be greater than 0");
        if (mass <= 0.0f) throw std::runtime_error("Mass must be greater than 0");

        // Use std::make_shared to create a shared pointer to a new Body
        return std::make_shared<Body>(radius, 0, 0, mass, position);
    }

    // User side functions to create a Box body
    std::shared_ptr<Body> Body::CreateRectangularBody(double width, double height, double mass, const Math::Vector& position)
    {
        if (width <= 0.0f || height <= 0.0f) throw std::runtime_error("Width and Height must be greater than 0");
        if (mass <= 0.0f) throw std::runtime_error("Mass must be greater than 0");

        return std::make_shared<Body>(0, width, height, mass, position);
    }

    // void Body::move(const FlatVector& amount) {

    //     position += amount;
    //     transformedUpdateRequired = true;
    // }

    // void Body::moveTo(const FlatVector& newPosition) {
    //     position = newPosition;
    //     transformedUpdateRequired = true;
    // }

    // void Body::rotate(double amount) {
    //     angle += amount;
    //     transformedUpdateRequired = true;
    // }

    // void Body::step(float time, const FlatVector& gravity) {
    //     // Calculate acceleration
    //     //FlatVector acceleration = force * inverseMass;

    //     if (isStatic) return;

    //     // Update linear velocity
    //     linearVelocity += gravity * time;
    //     // Update position
    //     position += linearVelocity * time;

    //     // Update angular velocity
    //     angle += angularVelocity * time;

    //     force = FlatVector::Zero;
    //     transformedUpdateRequired = true;
    // }

    // void Body::applyForce(const FlatVector& force) {
    //     this->force = force;
    // }

    // const std::vector<FlatVector>& Body::getTransformedVertices() {
    //     if (transformedUpdateRequired) {
    //         // Make a transformation object
    //         FlatTransformation transformation(position, angle);
    //         for (int i = 0; i < tranformedVertices.size(); i++) {
    //             // Check if vertices are not null
    //             if (vertices.size() > 0) {
    //                 const FlatVector& v = vertices[i];
    //                 tranformedVertices[i] = FlatVector::Transform(v, transformation);
    //             }
    //         }
    //     }
    //     transformedUpdateRequired = false;
    //     return tranformedVertices;
    // }

    // FlatAABB Body::getAABB() {
    //     double minX = FlatMath::FloatMax;
    //     double minY = FlatMath::FloatMax;
    //     double maxX = FlatMath::FloatMin;
    //     double maxY = FlatMath::FloatMin;

    //     if (shapeType == ShapeType::Circle) {
    //         minX = position.x - radius;
    //         minY = position.y - radius;

    //         maxX = position.x + radius;
    //         maxY = position.y + radius;
    //     }
    //     else if (shapeType == ShapeType::Box) {
    //         const std::vector<FlatVector>& t_vertices = getTransformedVertices();
    //         for (int i = 0; i < t_vertices.size(); i++) {
    //             const FlatVector& v = t_vertices[i];
    //             if (v.x < minX) minX = v.x;
    //             if (v.y < minY) minY = v.y;

    //             if (v.x > maxX) maxX = v.x;
    //             if (v.y > maxY) maxY = v.y;
    //         }
    //     }
    //     else {
    //         throw std::runtime_error("Invalid Shape Type");
    //     }

    //     return FlatAABB(minX, minY, maxX, maxY);

    // }

} // namespace FlatPhysics