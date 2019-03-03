#include "gapi/gapi.h"

void FPSCounter::begin(){
    fps_start = timer::now();
}

void FPSCounter::end()
{
    timer::time_point fps_end = timer::now();
    double ns = (double)(std::chrono::duration_cast<std::chrono::nanoseconds>(fps_end - fps_start).count());
    double s = ns / 1000000000.;

    this->runs_s.push_back(s);
    if (this->runs_s.size() > 100){
        this->runs_s.pop_front();
    }
    float avg_s = 0;
    for (int i = 0; i < runs_s.size(); i++){
       avg_s += runs_s[i];
    }
    avg_s /= runs_s.size();
    if (std::chrono::duration_cast<std::chrono::seconds>(timer::now() - this->last_print_time).count() > 1){
        this->last_print_time = timer::now();
        this->runs_s.clear();
//        fprintf(stdout, "FPS: %f, SPF: %f\n", 1 / static_cast<double>(avg_s), static_cast<double>(avg_s));
//        fflush(stdout);
    }
}
