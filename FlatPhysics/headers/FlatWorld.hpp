/*

    Represents the world

*/

#ifndef FLAT_WORLD_HPP
#define FLAT_WORLD_HPP

#include <vector>
// include pairs
#include <utility>
#include "../../FlatUtils/StopWatch.hpp"
#include "FlatContactResolver.hpp"


namespace FlatPhysics {

    // Forward Declaration of FlatBody, FlatVector, and FlatContactResolver
    class FlatBody;
    struct FlatVector;
    struct FlatContactResolver;



    class FlatWorld {
        std::vector<FlatBody*> bodies;
        std::vector<std::pair<int, int>> potentialCollisions;

    private:
        void seperateBodies(FlatBody* bodyA, FlatBody* bodyB, const FlatVector& minimumTranslationVector);

        void stepBodies(float time, size_t totalIterations);
        void findPotentialCollisions();
        void resolvePotentialCollisions();

        void resolveCollisionsBasic(const FlatContactResolver& resolver);
        void resolveCollisionsWithRotation(FlatContactResolver& resolver);


    public:
        // Define Constants
        static FlatVector Gravity;
        static constexpr float MinBodySize = 0.01f * 0.01f;
        static constexpr float MaxBodySize = 64.0f * 64.0f;

        static constexpr float MinDensity = 0.5f; // gm/cm^3
        static constexpr float MaxDensity = 21.45f; // gm/cm^3

        static constexpr size_t MinIterations = 1;
        static constexpr size_t MaxIterations = 128;


        // For Testing
        static FlatUtils::Stopwatch worldStopwatch;


        // Constructor
        FlatWorld() {}

        // Destructor
        ~FlatWorld();

        void addBody(FlatBody* body);
        void removeBody(int index);
        bool getBody(int index, FlatBody*& body);

        void step(float time, size_t iterations = MinIterations);
        size_t numBodies() const;

        void clear();

    };



} // namespace FlatPhysics


#endif // FLAT_WORLD_HPP