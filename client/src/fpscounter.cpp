#include "gapi.h"

void FPSCounter::measure(){
    timer::time_point fps_end = timer::now();
    double ns = (double)(std::chrono::duration_cast<std::chrono::nanoseconds>(fps_end - fps_start).count());
    double s = ns / 1000000000.;

    if (s > 2){
        fprintf(stdout, "FPS: %f, SPF: %f\n", static_cast<double>(i_frame) / s, s / static_cast<double>(i_frame) );
        fflush(stdout);
        fps_start = timer::now();
        i_frame = 0;
    }

    i_frame += 1;
}
