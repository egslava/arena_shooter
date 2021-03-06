#pragma once

#include "scene/camera.h"
#include "math.h"

class Program {
    uint _program = 0;
    std::vector<Shader> _shaders;   // will be removed automatically, after the destructor is called
public:
    Program(){}
    Program(const Program &that) = delete;
    Program(Program &&that);

    void delete_();

    /** Safe Program removal. Should be used only internally. A class user should just use destructor. */
    void _delete();

    void link(Shader &&vertex, Shader &&fragment);
    void transform(const float transform, float tx, float ty, float tz, float sx, float sy, float sz);

    void use(const Camera &camera, const Vec3 &ambient);
    void uniform(const char *name, float value);
    void uniform(const char *name, const Vec3 &value);
    void uniform(const char *name, const Mat4x4 &value);
    void set_mat_model(const Mat4x4 &mat);
    void set_mat_camera(const Mat4x4 &mat);
    void set_mat_projection(const Mat4x4 &mat);
    void set_color(const Vec3 &color);

//    void bind(){
//        glBindBuffer(GL_ARRAY_BUFFER, _shader);
//    }
//    void set_texture(const char *name, int uniform_location);

    ~Program();
};
