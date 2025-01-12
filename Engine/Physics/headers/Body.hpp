/*

    Represent a body in the simulation. The body will have properties defined by the user.
    I'll use an unordered map to store the properties of the body.

*/

#ifndef BODY_HPP
#define BODY_HPP

#include <unordered_map>
#include <string>
#include <memory>

// Forward Declaration of Math::Vector
namespace Math {
    struct Vector;
}

namespace Physics {

    class Body {

        // Physical Properties
        std::unordered_map<std::string, double> physicalProperties;

        // Kinematic Properties
        std::unordered_map<std::string, Math::Vector> kinematicProperties;

        std::unordered_map<std::string, bool> booleanProperties;

    public:

        Body(double radius, double height, double width, double mass, const Math::Vector& position);

        static std::shared_ptr<Body> CreateCircularBody(double radius, double mass, const Math::Vector& position);
        static std::shared_ptr<Body> CreateRectangularBody(double width, double height, double mass,
            const Math::Vector& position);


        void setPhysicalProperty(const std::string& property, float value);
        // void addPhysicalProperty(const std::string& property, float value);
        float getPhysicalProperty(const std::string& property) const;
        bool physicalPropertyExists(const std::string& property) const;

        void setKinematicProperty(const std::string& property, const Math::Vector& value);
        // void addKinematicProperty(const std::string& property, const Math::Vector& value);
        Math::Vector getKinematicProperty(const std::string& property) const;
        bool kinematicPropertyExists(const std::string& property) const;

    };
} // namespace Physics


#endif // BODY_HPP