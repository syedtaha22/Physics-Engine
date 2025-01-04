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
        std::vector<FlatBody*> bodies;

    public:
        // Define Constants
        static FlatVector Gravity;
        static constexpr float MinBodySize = 0.01f * 0.01f;
        static constexpr float MaxBodySize = 64.0f * 64.0f;

        static constexpr float MinDensity = 0.5f; // gm/cm^3
        static constexpr float MaxDensity = 21.45f; // gm/cm^3


        // Constructor
        FlatWorld() {}

        // Destructor
        ~FlatWorld();

        void addBody(FlatBody* body);
        void removeBody(int index);
        bool getBody(int index, FlatBody*& body);

        void step(float time);

        void resolveCollisions(FlatBody*& bodyA, FlatBody*& bodyB, FlatVector& normal, float depth);

        bool collides(FlatBody*& bodyA, FlatBody*& bodyB, FlatVector& normal, float& depth);

        size_t numBodies() const;

        void clear();

    };



} // namespace FlatPhysics


#endif // FLAT_WORLD_HPP