#include <iostream>
#include <chrono>

namespace pcmax {

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

        void displayTime(int option = 1) {
            switch (option)
            {
            case 0:
                std::cout << "Execution time : " << elapsedNanoSeconds() << "[ns]\n"; 
                break;
            case 1:
                std::cout << "Execution time : " << elapsedMilliseconds() << "[ms]\n";
                break;
            case 2:
                std::cout << "Execution time : " << elapsedSeconds() << "[s]\n";
                break;
            default:
                std::cout << "Execution time : " << elapsedMilliseconds() << "[ms]\n";
                break;
            }
        }

        double elapsedMilliseconds() {
            auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(this->endTime - this->startTime);
            return (double)duration.count() / 1e6;
        }

        double elapsedSeconds() const {
            auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(this->endTime - this->startTime);
            return (double)duration.count() / 1e9;
        }

        double elapsedNanoSeconds()const {
            auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(this->endTime - this->startTime);
            return (double)duration.count();
        }
    };
}