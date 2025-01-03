/*

    Represents the world

*/

#ifndef FLAT_WORLD_HPP
#define FLAT_WORLD_HPP

#include <vector>

namespace FlatPhysics {

    // Forward Declaration of FlatBody and FlatVector
    class FlatBody;
    struct FlatVector;


    class FlatWorld {
        static constexpr float gravity = 9.8f; // m/s^2

    public:
        // Define Constants
        static constexpr float minBodySize = 0.01f * 0.01f;
        static constexpr float maxBodySize = 64.0f * 64.0f;

        static constexpr float minDensity = 0.5f; // gm/cm^3
        static constexpr float maxDensity = 21.45f; // gm/cm^3

        std::vector<FlatBody*> bodies;

        // Constructor
        FlatWorld() {}

        // Destructor
        ~FlatWorld();

        void addBody(FlatBody* body);
        void removeBody(int index);
        bool getBody(int index, FlatBody*& body);

        void step(float time);

        bool collides(FlatBody*& bodyA, FlatBody*& bodyB, FlatVector& normal, float& depth);




    };



} // namespace FlatPhysics


#endif // FLAT_WORLD_HPP