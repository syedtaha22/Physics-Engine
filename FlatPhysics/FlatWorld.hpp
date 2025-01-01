/*

    Represent the world

*/

#ifndef FLATPHYSICS_FLATWORLD_HPP
#define FLATPHYSICS_FLATWORLD_HPP



namespace FlatPhysics {

    class FlatWorld {
    public:
        // Define Constants
        static constexpr float minBodySize = 0.01f * 0.01f;
        static constexpr float maxBodySize = 64.0f * 64.0f;

        static constexpr float minDensity = 0.5f; // gm/cm^3
        static constexpr float maxDensity = 21.45f; // gm/cm^3




    };



} // namespace FlatPhysics







#endif // FLATPHYSICS_FLATWORLD_HPP