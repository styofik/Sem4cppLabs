//
// Created by Stefan Postnikov on 08.02.2022.
//

#include <chrono>
#include <iostream>
#include <cmath>
#include <typeinfo>

using namespace std::chrono;

template<typename DurationType>
class Timer {
private:
    steady_clock::time_point checkpoint;
    DurationType result;
    bool is_paused;

    DurationType time_diff() {
        return duration_cast<DurationType>(steady_clock::now() - checkpoint);
    }
public:
    Timer(bool is_paused): 
            checkpoint(steady_clock::now()), 
            result(0), 
            is_paused(is_paused) {}
    Timer():  Timer(false) {}

    ~Timer() {
        std::cout << "Final time is " << get_result() << std::endl; 
    }
    void pause() {
        if (not is_paused) {
            result += time_diff();
            is_paused = true;
            std::cout << "Pause on " << get_result() << std::endl;
        }
    }
    void resume() {
        if (is_paused) {
            checkpoint = steady_clock::now();
            is_paused = false;
            std::cout << "Pause off" << std::endl;
        }
    }
    auto get_result() {
        return is_paused ? result.count() : (result + time_diff()).count();
    }
};

void useless_calcutaions() {
    double garbage = 0;
    for(auto i = 0u; i < 1000000u; i++) {
            garbage += std::sin(i) + std::cos(i);
    }
    std::cout << "One portion of useless calculations is proceeding" << std::endl;
}

int main() {
    Timer<std::chrono::milliseconds> test_timer;
    std::cout << "Duration in milliseconds" << std::endl;
    useless_calcutaions();
    test_timer.pause();
    useless_calcutaions();
    useless_calcutaions();
    test_timer.resume();
    useless_calcutaions();
    useless_calcutaions();
}