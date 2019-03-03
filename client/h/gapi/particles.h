#ifndef PARTICLES_H
#define PARTICLES_H

#include "gapi/loaders/mymodel.h"
#include "math/math.h"
#include "gapi/gapi.h"


struct Particle {
    Vec3 init_position;
    Vec3 init_velocity;
    Vec3 start_color, end_color;

    Vec3 angle;  // 0 - start angle, 1 - begin velocity, 2 - end velocity


    float start_size, end_size;
    double start_time = -1;
    double end_time = -1;
};

enum class EmitterType: int { EXPLOSION,  FOUNTAIN };

struct Emitter {
    EmitterType type;
    int max_particles;
    double base_time;
    float min_live_time;
    float max_live_time;
    float position_range;

    float min_start_size, max_start_size, min_end_size, max_end_size;

    float min_start_angle, max_start_angle;
    float min_start_angular_velocity, max_start_angular_velocity;
    float min_end_angular_velocity, max_end_angular_velocity;

    Ball velocity_range;
    Ball start_color_range;
    Ball end_color_range;

    Vec3 gravity;

    double time() const;
    Emitter();
};


class Particles {
    static constexpr int _POINTS_PER_PRIMITIVE = 6;
    VAO _vao;
    std::vector<Particle> particles;
    Program particle_shader;

    std::vector<float> position_buffer;
    std::vector<float> angle_buffer;
    std::vector<float> size_buffer;
    std::vector<float> velocity_buffer;
    std::vector<float> start_color_buffer;
    std::vector<float> end_color_buffer;
    std::vector<float> time_buffer;

    void _link_programs();
    void _resize_vectors();
    void _particles_to_vectors();

    // everything for Node class
    friend class Node;

    float _uniform_emitter_time;    // time since the Particle system has been created
    int num_dead = 0;  // all particles are active
    bool should_explode = false;  // should explode during this update
public:
    Emitter emitter;
    Texture _tex;

//    Model(VAO &&vao): _vao(std::move(vao)) {}

    void update(const Vec3 &pos, bool is_visible);
    void draw(const Camera &camera, const Vec3 &ambient_color);
    void vectors2vbos();

    Particles(){}
    Particles(const Particles&) = delete;
    Particles(Particles &&that) = default;
    void init(const Vec3 &pos);

    void update_particles_state(const Vec3 &position, bool update);
    void explode();
};

#endif // PARTICLES_H
