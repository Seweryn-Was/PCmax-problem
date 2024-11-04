#include <iostream>
#include <chrono>

class Timer {
private:
    std::chrono::high_resolution_clock::time_point startTime;
    std::chrono::high_resolution_clock::time_point endTime;
public:
    Timer() {
        this->startTime = std::chrono::high_resolution_clock::now(); // Initialize start time
    }

    void start() {
        this->startTime = std::chrono::high_resolution_clock::now(); // Start/restart timer
    }

    void stop() {
        this->endTime = std::chrono::high_resolution_clock::now(); // Stop the timer
    }

    double elapsedMilliseconds() {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(this->endTime - this->startTime);
        return duration.count() / 1e6;
    }

    double elapsedSeconds() const {
        return std::chrono::duration_cast<std::chrono::seconds>(this->endTime - this->startTime).count();
    }

    double elapsedNanoSeconds()const{
        return std::chrono::duration_cast<std::chrono::nanoseconds>(this->endTime - this->startTime).count();
    }

};
