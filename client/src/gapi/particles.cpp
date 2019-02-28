#include "gapi/gapi.h"

#include <chrono>


const char *particles_vertex_shader_code =
    "#version 330 core\n"
    "layout (location = 0) in vec3 _init_pos;\n"
    "layout (location = 1) in vec3 _in_w;\n"
    "layout (location = 2) in vec2 _init_size;\n"
    "layout (location = 3) in vec3 _init_velocity;\n"
    "layout (location = 4) in vec4 _start_color;\n"
    "layout (location = 5) in vec4 _end_color;\n"
    "layout (location = 6) in vec2 _particle_timing;\n"
    "out vec2  _tex_coords;"
    "out vec4  _interpolated_color;"
    "uniform mat4 model;\n"
    "uniform mat4 projection;\n"
    "uniform mat4 camera;\n"
    "uniform float _time; \n"  // time %= 24x3600, eps = (2**17-2**16)/2**23 = 0.0078125
    "uniform vec4 _gravity;\n"
    "float _start_time = _particle_timing[0];\n"
    "float _end_time = _particle_timing[1];\n"
    "float _start_size = _init_size[0];\n"
    "float _end_size = _init_size[1];\n"
    "float normed_time = (_time - _start_time) / (_end_time - _start_time);\n"
    "const float _sp = 1.0;  // shift power \n"
    "const vec4 _shift[6] = vec4[6](\n"
    "    vec4( _sp,  _sp, 0, 0), vec4(-_sp,  _sp, 0, 0), vec4(-_sp, -_sp, 0, 0), \n"
    "    vec4(-_sp, -_sp, 0, 0), vec4( _sp, -_sp, 0, 0), vec4( _sp,  _sp, 0, 0));\n"
    "\n"

    "const float _angles[6] = float[6](\n"
    "    45, 135, -135, \n"
    "    -135, -45, 45 );\n"
    "\n"
    "const vec2 _texcoords[6] = vec2[6](\n"
    "    vec2(   1,    1), vec2( 0,  1), vec2( 0,  0), \n"
    "    vec2(   0,    0), vec2( 1,  0), vec2( 1,  1));\n"
    "\n"
    "float angular_velocity = mix(_in_w.y, _in_w.z, normed_time);\n"
    "float angle = _in_w.x + angular_velocity*normed_time + _angles[gl_VertexID % 6] / 180 * 3.141529;\n"
    "vec4 rot = vec4(cos(angle), sin(angle), 0, 0);\n"
    "void main() {\n"
    "    _interpolated_color = mix(_start_color, _end_color, normed_time);\n"
    "    vec4 pos = vec4(_init_pos + _init_velocity*normed_time, 1)  + vec4( (_gravity*normed_time*normed_time).xyz,0);\n"
//    "    float size = normed_time;//0.5+t*3;\n"
    "    float size = mix(_start_size, _end_size, normed_time);\n"
//    "    float size = _start_size;\n"
    "    pos = camera * pos;\n"
    "    pos +=  size * rot;\n" // _shift[gl_VertexID % 6] *
    "    pos = projection * pos;\n"
    "    gl_Position = pos;\n"
    "    _tex_coords = _texcoords[gl_VertexID % 6];\n"
    "}\n"
     ;
const char *particles_fragment_shader_code =
    "#version 330 core\n"
    "uniform sampler2D texture0;\n"
    "uniform vec4  _ambient_color;\n"
    "in vec2 _tex_coords;\n"
    "in vec4  _interpolated_color;\n"
    "out vec4 FragColor;\n"
    "\n"
    "void main() {\n"
    "    FragColor = _ambient_color + _interpolated_color * texture(texture0, _tex_coords);\n"
//      "    FragColor = vec4(1,1,1,1); //_ambient_color +  * texture(texture0, _tex_coords);\n"
    "}\n";



double now(){
    using timer = std::chrono::high_resolution_clock;
    using ns = std::chrono::nanoseconds;
    return static_cast<double>(std::chrono::duration_cast<ns>( timer::now().time_since_epoch() ).count()) / 1e9;
}

void reborn(const Ball &position_range, const float emitter_time, const Emitter &emitter, Particle &particle, bool random_init_time){
//    if (random_init_time){
//        particle.init_time = rand(emitter.time(), emitter.time() + emitter.max_live_time);
//    } else {
//        particle.init_time = emitter.time();
//    }
//    particle.end_time = rand(particle.init_time, particle.init_time + emitter.max_live_time);

    float init_time = emitter_time;
    float end_time = init_time + rand(emitter.min_live_time, emitter.max_live_time);

    if (random_init_time){
        float current_normed_time = rand(0.0f, 1.0f);
        float current_time_from_init = (end_time - init_time) * current_normed_time;
        end_time -= current_time_from_init;
        init_time -= current_time_from_init;
    }

    particle.start_time = init_time;
    particle.end_time = end_time;
    particle.init_position = rand(position_range);
    particle.angle = Vec3(
                rand(emitter.min_start_angle, emitter.max_start_angle),
                rand(emitter.min_start_angular_velocity, emitter.max_start_angular_velocity),
                rand(emitter.min_end_angular_velocity, emitter.min_end_angular_velocity));
    particle.init_velocity = rand(emitter.velocity_range);
    particle.start_color = rand(emitter.start_color_range);
    particle.end_color = rand(emitter.end_color_range);
    particle.start_size = rand(emitter.min_start_size, emitter.max_start_size);
    particle.end_size = rand(emitter.min_end_size, emitter.max_end_size);
}

bool is_dead(float time, const Emitter &emitter, const Particle &particle){
//    return (emitter.time() - particle.init_time > emitter.max_live_time );
    return (time > particle.end_time);
}

void Particles::update_particles_state(const Vec3 &position, bool update) {
    bool kill = !update || this->should_explode;
    bool random_init_time = !update && this->emitter.type == EmitterType::FOUNTAIN;

    Ball position_range{ position, this->emitter.position_range };

    if (kill){
        this->num_dead = 0;
        for (Particle &particle : particles)
            reborn(position_range, this->_uniform_emitter_time, this->emitter, particle, random_init_time);
        return;
    }

    switch (emitter.type) {
    case EmitterType::EXPLOSION:
        for (int i = 0; i < particles.size() - this->num_dead; i++){
            Particle &particle = particles[i];
            // should_explode should influence on 'update'. So, it should just respawn
            // the whole Particle System and that's all
            if (is_dead(this->_uniform_emitter_time, this->emitter, particle)){
                this->num_dead += 1;
                if (this->num_dead == this->particles.size())
                    return;

                particle = particles[particles.size() - 1 - num_dead];
//                reborn(position, this->_uniform_emitter_time, this->emitter, particle, false);
            }
        }
        break;
    case EmitterType::FOUNTAIN:{
        for (Particle &particle : particles) {
            if (is_dead(this->_uniform_emitter_time, this->emitter, particle)){
                reborn(position_range, this->_uniform_emitter_time, this->emitter, particle, random_init_time);
            }
        }
    } break;
#ifndef NDEBUG
    default:
        throw MyIllegalStateException("Emitter type can be only EXPLOSION or FOUNTAIN");
#endif
    }
}


double Emitter::time() const {
    return now() - base_time;
}

Emitter::Emitter():
    base_time( now() ),
    min_live_time(0),
    max_live_time(1),
    max_particles(1),
    position_range(0),
    min_start_size(0),
    max_start_size(0),
    min_end_size(1),
    max_end_size(3),
    gravity(0, 0, 0),
    type(EmitterType::EXPLOSION)
{
    this->velocity_range.C = Vec3(0, 2, 0);
    this->velocity_range.R = 1.0;
}

void Particles::_link_programs(){
    Shader vertex_shader, fragment_shader;
    vertex_shader.compile(Shader::Type::VERTEX_SHADER, particles_vertex_shader_code);
    fragment_shader.compile(Shader::Type::FRAGMENT_SHADER, particles_fragment_shader_code);
    particle_shader.link(std::move(vertex_shader), std::move(fragment_shader) );
}

void Particles::_resize_vectors(){
    position_buffer.resize(particles.size() * _POINTS_PER_PRIMITIVE * 3);  // 6 points with 3 coords
    angle_buffer.resize(particles.size() * _POINTS_PER_PRIMITIVE * 3);
    size_buffer.resize(particles.size() * _POINTS_PER_PRIMITIVE * 2);
    velocity_buffer.resize(particles.size() * _POINTS_PER_PRIMITIVE * 3);
    start_color_buffer.resize(particles.size() * _POINTS_PER_PRIMITIVE * 4);
    end_color_buffer.resize(particles.size() * _POINTS_PER_PRIMITIVE * 4);
    time_buffer.resize(particles.size() * _POINTS_PER_PRIMITIVE * 2);
}

void Particles::_particles_to_vectors() {

    for (int i_part = 0; i_part < this->particles.size(); i_part++) {
        const Particle &particle = this->particles[i_part];
        for(int i_vertex = 0; i_vertex < _POINTS_PER_PRIMITIVE; i_vertex++){
            position_buffer[i_part*_POINTS_PER_PRIMITIVE*3 + i_vertex * 3 + 0] = particle.init_position._x;
            position_buffer[i_part*_POINTS_PER_PRIMITIVE*3 + i_vertex * 3 + 1] = particle.init_position._y;
            position_buffer[i_part*_POINTS_PER_PRIMITIVE*3 + i_vertex * 3 + 2] = particle.init_position._z;

            angle_buffer[i_part*_POINTS_PER_PRIMITIVE*3 + i_vertex * 3 + 0] = particle.angle._x;
            angle_buffer[i_part*_POINTS_PER_PRIMITIVE*3 + i_vertex * 3 + 1] = particle.angle._y;
            angle_buffer[i_part*_POINTS_PER_PRIMITIVE*3 + i_vertex * 3 + 2] = particle.angle._z;

            velocity_buffer[i_part*_POINTS_PER_PRIMITIVE*3 + i_vertex * 3 + 0] = particle.init_velocity._x;
            velocity_buffer[i_part*_POINTS_PER_PRIMITIVE*3 + i_vertex * 3 + 1] = particle.init_velocity._y;
            velocity_buffer[i_part*_POINTS_PER_PRIMITIVE*3 + i_vertex * 3 + 2] = particle.init_velocity._z;

            start_color_buffer[i_part*_POINTS_PER_PRIMITIVE*4 + i_vertex * 4 + 0] = particle.start_color._x;
            start_color_buffer[i_part*_POINTS_PER_PRIMITIVE*4 + i_vertex * 4 + 1] = particle.start_color._y;
            start_color_buffer[i_part*_POINTS_PER_PRIMITIVE*4 + i_vertex * 4 + 2] = particle.start_color._z;
            start_color_buffer[i_part*_POINTS_PER_PRIMITIVE*4 + i_vertex * 4 + 3] = particle.start_color._w;

            end_color_buffer[i_part*_POINTS_PER_PRIMITIVE*4 + i_vertex * 4 + 0] = particle.end_color._x;
            end_color_buffer[i_part*_POINTS_PER_PRIMITIVE*4 + i_vertex * 4 + 1] = particle.end_color._y;
            end_color_buffer[i_part*_POINTS_PER_PRIMITIVE*4 + i_vertex * 4 + 2] = particle.end_color._z;
            end_color_buffer[i_part*_POINTS_PER_PRIMITIVE*4 + i_vertex * 4 + 3] = particle.end_color._w;

            time_buffer[i_part*_POINTS_PER_PRIMITIVE*2 + i_vertex * 2 + 0] = particle.start_time;
            time_buffer[i_part*_POINTS_PER_PRIMITIVE*2 + i_vertex * 2 + 1] = particle.end_time;

            size_buffer[i_part*_POINTS_PER_PRIMITIVE*2 + i_vertex * 2 + 0] = particle.start_size;
            size_buffer[i_part*_POINTS_PER_PRIMITIVE*2 + i_vertex * 2 + 1] = particle.end_size;
        }
    }

}

void Particles::init(const Vec3 &pos){
//    this->_tex = std::move(texture);

    particles.resize(emitter.max_particles);
    this->_uniform_emitter_time = emitter.time();
    update_particles_state(pos, false);
    this->_resize_vectors();
    this->_particles_to_vectors();
    std::vector<VBO> gpu_vbos;
    gpu_vbos.push_back( VBO().data(position_buffer, 3) );
    gpu_vbos.push_back( VBO().data(angle_buffer, 3) );
    gpu_vbos.push_back( VBO().data(size_buffer, 2 ) );
    gpu_vbos.push_back( VBO().data(velocity_buffer, 3) );
    gpu_vbos.push_back( VBO().data(start_color_buffer, 4) );
    gpu_vbos.push_back( VBO().data(end_color_buffer, 4) );
    gpu_vbos.push_back( VBO().data(time_buffer, 2) );

    this->_vao.data( std::move(gpu_vbos) );
    _link_programs();
}

void Particles::explode()
{
#ifndef NDEBUG
    if (this->emitter.type != EmitterType::EXPLOSION){
        throw MyIllegalStateException("Only particles of type EXPLOSION can be exploded!");
    }
#endif
    this->should_explode = true;
}

void Particles::update(const Vec3 &pos)
{
    this->_uniform_emitter_time = emitter.time();

    update_particles_state(pos, true);
    this->should_explode = false;
    this->_particles_to_vectors();
    this->vectors2vbos();
}

void Particles::vectors2vbos(){
    this->_vao._vbos[0].data(position_buffer, 3);
    this->_vao._vbos[1].data(angle_buffer, 3);
    this->_vao._vbos[2].data(size_buffer, 2);
    this->_vao._vbos[3].data(velocity_buffer, 3);
    this->_vao._vbos[4].data(start_color_buffer, 4);
    this->_vao._vbos[5].data(end_color_buffer, 4);
    this->_vao._vbos[6].data(time_buffer, 2);
}

void Particles::draw(const Camera &camera, const Vec3 &ambient_color) {
    this->particle_shader.use(camera, ambient_color);
    this->particle_shader.uniform("_time", this->_uniform_emitter_time);
    this->particle_shader.uniform("_ambient_color", ambient_color);
    this->particle_shader.uniform("_gravity", this->emitter.gravity);
//    vectors2vbos();
    this->_vao.bind();
    this->_tex.bind();

    glDepthMask(GL_FALSE);
//    printf("%d == particles.size()\n", this->particles.size());
    glDrawArrays(GL_TRIANGLES, 0, (this->particles.size() - this->num_dead) * 2 * 3);
    glDepthMask(GL_TRUE);
}
