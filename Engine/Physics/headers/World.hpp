#ifndef WORLD_HPP
#define WORLD_HPP

#include <vector>
#include <memory>


namespace Physics {

    class Body;

    class World {
        std::vector<std::shared_ptr<Body>> bodies;

        static constexpr double G = 6.67430e-11f;

    private:
        void calculateGravitionalForce(std::shared_ptr<Body> body1, std::shared_ptr<Body> body2);

    public:
        void addBody(std::shared_ptr<Body> body);
        std::shared_ptr<Body> getBody(int index);
        void removeBody(std::shared_ptr<Body> body);

        size_t numBodies() const;

        void step(double time);

        void calculateGForces();






    };

} // namespace Physics






#endif // WORLD_HPP