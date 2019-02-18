#include "gapi.h"
#include "math/matrix.h"

Program::Program(Program &&that){
    _program = that._program;
    _shaders = std::move(that._shaders);
    that._program = 0;
}


void Program::delete_(){
#ifndef NDEBUG
    if (_program == 0){
        throw MyIllegalStateException("An attempt to delete a Program without calling glCreateProgram() first.");
    }
#endif

    _shaders.clear();  // call the destructors and remove all the shaders

    glDeleteProgram(_program);
    _program = 0;
}

void Program::_delete(){
    if(_program != 0) delete_();
}

Mat4x4 matModel = Mat4x4::I;

void Program::link(Shader &&vertex, Shader &&fragment){
    matModel = Mat4x4::I;
    if (_program != 0) delete_();

    _shaders.push_back(std::move(vertex));
    _shaders.push_back(std::move(fragment));

    _program = glCreateProgram();
    if (_program == 0){
        throw MyGlException("Can't create a Program ");
    }

//    use();


    for (const auto & shader : _shaders){
        glAttachShader(_program, shader._shader);
    }
    glLinkProgram(_program);

    int success;
    char info_log[512];
    glGetProgramiv(_program, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(_program, 512, NULL, info_log);
        throw MyShaderLinkException(info_log);
    }
}

float _angle = 0.0;
float _tx = 0.0, _ty=0, _tz = 0.0;


void Program::transform(const float angleOY, float tx, float ty, float tz, float sx, float sy, float sz)
{
    _tx = tx;
    _ty = ty;
    _tz = tz;

//    printf("%0.2f, %0.2f, %0.2f\n", _tx, _ty, _tz);

    _angle = angleOY;

    float speed = 0.0001;
    if (_angle < 0){
        speed *= -1;
    }

    Mat4x4 mRot = Mat4x4::set_rot_y(_angle);
    Mat4x4 mTranslation = Mat4x4::set_translation(_tx, _ty, tz);
    Mat4x4 mScale = Mat4x4::set_scale(sx, sy, sz);
//    Mat4x4 mTranslation = Mat4x4::set_scale(_tx, _tz, 1);


//    Mat4x4 m = Mat4x4::I;
//    rot = m * rot;
//    matModel = mTranslation; //mTranslation * mRot;
    matModel = mTranslation * mScale * mRot;
//    matModel = mRot; //mTranslation * mRot;
//    matModel = mTranslation;
}


float angle = 0;
//    angle += 0.0001;

#include <math.h>

void Program::use(const Camera &camera){

//    m *= 0.5;
//    m.set_rot_y(angle);

    glUseProgram(_program);

    GLint loc_mat_model = glGetUniformLocation(_program, "model");
#ifndef NDEBUG
    if (loc_mat_model < 0) throw MyIllegalStateException("glGetUniformLocation returns -1");
#endif
    glUniformMatrix4fv(loc_mat_model, 1, GL_FALSE, matModel.T()._data);



//    Mat4x4 mat_projection = Mat4x4::set_perspective(60.0f / 180.0f * 3.141529, 640.0f/480.0f, 0.001f, 100.00f);
    Mat4x4 mat_projection = Mat4x4::set_perspective(90.0f / 180.0f * 3.141529, 640.0f/480.0f, 0.001f, 900.00f);
    GLint loc_mat_projection = glGetUniformLocation(_program, "projection");
#ifndef NDEBUG
    if (loc_mat_projection < 0) throw MyIllegalStateException("glGetUniformLocation returns -1");
#endif
    glUniformMatrix4fv(loc_mat_projection, 1, GL_FALSE, mat_projection.T()._data);



    GLint loc_mat_camera = glGetUniformLocation(_program, "camera");
#ifndef NDEBUG
    if (loc_mat_camera < 0) throw MyIllegalStateException("glGetUniformLocation returns -1");
#endif
    glUniformMatrix4fv(loc_mat_camera, 1, GL_FALSE, camera.getMatWorldToCamera().T()._data);


    GLint loc_tex0 = glGetUniformLocation(_program, "texture0");
    glUniform1i(loc_tex0, 0);
}

void Program::set_color(const Vec3 &color) {
    GLint loc_mat_color = glGetUniformLocation(_program, "mat_color");
#ifndef NDEBUG
    if (loc_mat_color < 0) throw MyIllegalStateException("glGetUniformLocation returns -1");
#endif


    GLfloat color4f[4];
    color.to_f4(color4f);
    glUniform4fv(loc_mat_color, 1, color4f);
}

//void Shader::set_texture(const char *name, int uniform_location)
//{

//}


Program::~Program(){
    _delete();
}

