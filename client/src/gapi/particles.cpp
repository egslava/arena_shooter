#include "gapi/gapi.h"

#include <chrono>


const char *particles_vertex_shader_code =
    "#version 330 core\n"
    "layout (location = 0) in vec3 _init_pos;\n"
    "layout (location = 1) in vec3 _init_velocity;\n"
    "layout (location = 2) in float _start_time;\n"
    "out float _tt;"
    "uniform mat4 model;\n"
    "uniform mat4 projection;\n"
    "uniform mat4 camera;\n"
    "uniform float _time;  // time %= 24x3600, eps = (2**17-2**16)/2**23 = 0.0078125\n"
    "\n"
    "//\n"
    "const float _sp = 0.5;  // shift power \n"
    "const vec4 _shift[6] = vec4[6](\n"
    "    vec4( _sp,  _sp, 0, 0), vec4(-_sp,  _sp, 0, 0), vec4(-_sp, -_sp, 0, 0), \n"
    "    vec4(-_sp, -_sp, 0, 0), vec4( _sp, -_sp, 0, 0), vec4( _sp,  _sp, 0, 0));\n"
    "\n"
    "\n"
    "void main() {\n"
    "    float t = _time - _start_time;\n"
    "    vec4 pos = vec4(_init_pos + _init_velocity*t  + vec3(0,  -9.8/2*t*t, 0), 1);\n"
    "\n"
    "    pos = camera * pos;\n"
    "    pos += _shift[gl_VertexID % 6];\n"
    "    pos = projection * pos;\n"
    "    _tt = sin(2./100.*t);\n"
    "    gl_Position = pos;\n"
    "}\n"
     ;

const char *particles_fragment_shader_code =
    "#version 330 core\n"
    "in float _tt;\n"
    "out vec4 FragColor;\n"
    "\n"
    "void main() {\n"
    "    FragColor = vec4(_tt, 1, 1, 1);\n"
    "}\n";











double now(){
    using timer = std::chrono::high_resolution_clock;
    using ns = std::chrono::nanoseconds;
    return static_cast<double>(std::chrono::duration_cast<ns>( timer::now().time_since_epoch() ).count()) / 1e9;
}

void reborn(const Emitter &emitter, Particle &particle, bool reinit) {
    if (reinit){
        particle.init_time = emitter.time();
    }
    else {
        particle.init_time = rand(emitter.time(), emitter.time() + emitter.max_live_time);
    }
    particle.init_position = rand(emitter.position_range);
    particle.init_velocity = rand(emitter.velocity_range);
    particle.init_start_color = rand(emitter.start_color_range);
    particle.init_end_color = rand(emitter.end_color_range);
}

bool is_dead(const Emitter &emitter, Particle &particle){
    return (emitter.time() - particle.init_time > emitter.max_live_time );
}

void init_particles(std::vector<Particle> &particles, const Emitter &emitter, bool reinit=false){
    for (Particle &particle : particles) {
        if (is_dead(emitter, particle)){
            reborn(emitter, particle, reinit);
        }
    }

}


double Emitter::time() const {
    return now() - base_time;
}

Emitter::Emitter():
    base_time(now() ),
    max_live_time(1)
{
    this->position_range.C = Vec3(0, 4, 0);
    this->position_range.R = 0.5;
    this->velocity_range.C = Vec3(0, 0, 0);
    this->velocity_range.R = 0.1;
}

void Particles::_init_program(){
    Shader vertex_shader, fragment_shader;
    vertex_shader.compile(Shader::Type::VERTEX_SHADER, particles_vertex_shader_code);
    fragment_shader.compile(Shader::Type::FRAGMENT_SHADER, particles_fragment_shader_code);
    particle_shader.link(std::move(vertex_shader), std::move(fragment_shader) );
}

void Particles::_init_buffers(){
    position_buffer.resize(particles.size() * 6 * 3);  // 6 points with 3 coords
    velocity_buffer.resize(particles.size() * 6 * 3);
    time_buffer.resize(particles.size() * 6);
}

void Particles::_update_buffers() {
    constexpr int quads = 6;

    for (int i_part = 0; i_part < this->particles.size(); i_part++) {
        const Particle &particle = this->particles[i_part];
        for(int i_vertex = 0; i_vertex < 6; i_vertex++){
            position_buffer[i_part*quads*3 + i_vertex * 3 + 0] = particle.init_position._x;
            position_buffer[i_part*quads*3 + i_vertex * 3 + 1] = particle.init_position._y;
            position_buffer[i_part*quads*3 + i_vertex * 3 + 2] = particle.init_position._z;

            velocity_buffer[i_part*quads*3 + i_vertex * 3 + 0] = particle.init_velocity._x;
            velocity_buffer[i_part*quads*3 + i_vertex * 3 + 1] = particle.init_velocity._y;
            velocity_buffer[i_part*quads*3 + i_vertex * 3 + 2] = particle.init_velocity._z;

            time_buffer[i_part*quads + i_vertex] = particle.init_time;
        }
    }

}

void Particles::init(int n_particles){
    particles.resize(n_particles);
    init_particles(this->particles, this->emitter);
    _init_buffers();
    _init_program();
    _update_buffers();
}

bool first_time = true;
void Particles::draw(const Camera &camera) {
    if (first_time){
        first_time = false;
        this->_vao.data(VBO().data(position_buffer, 3), VBO().data(velocity_buffer, 3), VBO().data(time_buffer, 1));
    }
    init_particles(this->particles, this->emitter, true);
    _update_buffers();


    this->_vao._vbos[0].data(position_buffer, 3);
    this->_vao._vbos[1].data(velocity_buffer, 3);
    this->_vao._vbos[2].data(time_buffer, 1);

//    this->_tex.bind();
    this->_vao.bind();
    this->particle_shader.use(camera, Vec3(1,1,1));
//    printf("Emitter time: %f\n", emitter.time());
    this->particle_shader.uniform("_time", static_cast<float>(emitter.time()));
    glDrawArrays(GL_TRIANGLES, 0, this->particles.size() * 2 * 3);
}
