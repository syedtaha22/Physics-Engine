#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP


namespace Physics {

    class Constants {
    public:
        // Orbital Mechanics
        static constexpr double GRAVITATIONAL_CONSTANT = 6.67430e-11;  // Gravitational constant, m^3 kg^-1 s^-2
        static constexpr double STANDARD_GRAVITY = 9.80665;      // Standard gravity, m/s^2
        static constexpr double SPEED_OF_LIGHT = 299792458.0;  // Speed of light, m/s
        static constexpr double ASTRONOMICAL_UNIT = 1.496e11;    // Astronomical Unit, 1AU = 1.496e11 m

        // Rigid Body Dynamics
        static constexpr double AIR_DENSITY = 1.225;     // Air density at sea level, kg/m^3
        static constexpr double WATER_DENSITY = 1000.0;  // Water density, kg/m^3
    };
}

#endif // CONSTANTS_HPP