#ifndef PARTICLES_H
#define PARTICLES_H

#include "gapi/loaders/mymodel.h"
#include "math/math.h"
#include "gapi/gapi.h"


struct Particle {
    Vec3 init_position;
    Vec3 init_velocity;
    Vec3 init_start_color;
    Vec3 init_end_color;
    double init_time = -1;
};

struct Emitter {
    double base_time;
    float max_live_time;
    Ball position_range;
    Ball velocity_range;
    Ball start_color_range;
    Ball end_color_range;

    double time() const;
    Emitter();
};


class Particles {
    static constexpr int _POINTS_PER_PRIMITIVE = 6;
    Emitter emitter;
    VAO _vao;
    Texture _tex;
    std::vector<Particle> particles;
    Program particle_shader;

    std::vector<float> position_buffer;
    std::vector<float> velocity_buffer;
    std::vector<float> time_buffer;

    void _init_program();
    void _init_buffers();
    void _update_buffers();
public:

    void init(int n_particles);

    Particles(){}
    Particles(const Particles&) = delete;
    Particles(Particles &&that) = default;
//    Model(VAO &&vao): _vao(std::move(vao)) {}

    void draw(const Camera &camera);
};

#endif // PARTICLES_H
