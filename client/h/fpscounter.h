#pragma once


struct FPSCounter {
    using timer = std::chrono::high_resolution_clock;
    int i_frame = 0;
    timer::time_point fps_start = timer::now();

    void measure();
};
