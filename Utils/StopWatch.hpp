/*

    Represent a simple stopwatch that can be used to measure time intervals.

*/


#ifndef STOPWATCH_HPP
#define STOPWATCH_HPP

#include <chrono>
#include <thread>

namespace Utils {


    class Stopwatch {
    private:
        std::chrono::steady_clock::time_point start_time;
        std::chrono::steady_clock::time_point end_time;
        bool running;

    public:
        Stopwatch() : running(false) {}

        // Start the stopwatch
        void start() {
            if (!running) {
                start_time = std::chrono::steady_clock::now();
                running = true;
            }
        }

        // Stop the stopwatch
        void stop() {
            if (running) {
                end_time = std::chrono::steady_clock::now();
                running = false;
            }
        }

        // Reset the stopwatch
        void reset() {
            running = false;
            start_time = std::chrono::steady_clock::time_point(); // Set to 0
            end_time = std::chrono::steady_clock::time_point();   // Set to 0
        }

        // Restart the stopwatch
        void restart() {
            reset();
            start();
        }

        // Get elapsed time in seconds
        double getElapsedTimeInSeconds() {
            if (running) {
                return std::chrono::duration<double>(std::chrono::steady_clock::now() - start_time).count();
            }
            return std::chrono::duration<double>(end_time - start_time).count();
        }

        // Get elapsed time in milliseconds as a double
        double getElapsedTimeInMilliseconds() {
            if (running) {
                return std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - start_time).count();
            }
            return std::chrono::duration<double, std::milli>(end_time - start_time).count();
        }
    };




} // namespace Utils


#endif // STOPWATCH_HPP