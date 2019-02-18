#pragma once

#include <stack>
#include <deque>
struct FPSCounter {
    std::deque<float> runs_s;

    using timer = std::chrono::high_resolution_clock;
    timer::time_point fps_start = timer::now();
    timer::time_point last_print_time = timer::now();

    void begin();
    void end();
};
