/*

    Represents the world

*/

#ifndef FLAT_WORLD_HPP
#define FLAT_WORLD_HPP

#include <vector>
#include "../../FlatUtils/StopWatch.hpp"
#include "FlatManifold.hpp"


namespace FlatPhysics {

    // Forward Declaration of FlatBody, FlatVector, and FlatManifold
    class FlatBody;
    struct FlatVector;
    struct FlatManifold;



    class FlatWorld {
        std::vector<FlatBody*> bodies;
        std::vector<FlatManifold> contactList;

    public:
        // Define Constants
        static FlatVector Gravity;
        static constexpr float MinBodySize = 0.01f * 0.01f;
        static constexpr float MaxBodySize = 64.0f * 64.0f;

        static constexpr float MinDensity = 0.5f; // gm/cm^3
        static constexpr float MaxDensity = 21.45f; // gm/cm^3

        static constexpr size_t MinIterations = 1;
        static constexpr size_t MaxIterations = 128;

        static FlatUtils::Stopwatch worldStopwatch;


        // Constructor
        FlatWorld() {}

        // Destructor
        ~FlatWorld();

        void addBody(FlatBody* body);
        void removeBody(int index);
        bool getBody(int index, FlatBody*& body);

        void step(float time, size_t iterations = MinIterations);

        void resolveCollisions(const FlatManifold& collisionManifold);

        bool collides(FlatBody*& bodyA, FlatBody*& bodyB, FlatVector& normal, float& depth);

        size_t numBodies() const;

        void clear();

    };



} // namespace FlatPhysics


#endif // FLAT_WORLD_HPP