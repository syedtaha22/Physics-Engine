#ifndef PATH_HPP
#define PATH_HPP

#include <array>

namespace Math {
    struct Vector;
}

namespace Physics {

    template <std::size_t Capacity=5000>
    class Path {
    private:
        std::array<Math::Vector, Capacity> buffer{};
        std::size_t head = 0;   // Next write position
        std::size_t size = 0;   // Number of valid points

    public:
        Path() = default;

        // Insert newest point (overwrites oldest when full)
        inline void insert(const Math::Vector& v) {
            buffer[head] = v;
            head = (head + 1) % Capacity;
            if (size < Capacity) ++size;
        }

        // Access i-th element (0 = oldest, size-1 = newest)
        inline const Math::Vector& get(std::size_t i) const {
            std::size_t index = (head + Capacity - size + i) % Capacity;
            return buffer[index];
        }

        inline std::size_t getSize() const {
            return size;
        }

        inline constexpr std::size_t getCapacity() const {
            return Capacity;
        }

        inline bool isFull() const {
            return size == Capacity;
        }

        inline void clear() {
            head = 0;
            size = 0;
        }
    };

} // namespace Physics

#endif // PATH_HPP
