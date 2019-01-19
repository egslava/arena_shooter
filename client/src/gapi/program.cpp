#include "gapi.h"

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

void Program::link(Shader &&vertex, Shader &&fragment){
    if (_program != 0) delete_();

    _shaders.push_back(std::move(vertex));
    _shaders.push_back(std::move(fragment));

    _program = glCreateProgram();
    if (_program == 0){
        throw MyGlException("Can't create a Program ");
    }

    glUniform1i(glGetUniformLocation(fragment._shader, "texture0"), 0);

    for (const auto & shader : _shaders){
        glAttachShader(_program, shader._shader);
    }
    glLinkProgram(_program);

    int success;
    char info_log[512];
    glGetProgramiv(_program, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(_program, 512, NULL, info_log);
        throw MyShaderException(info_log);
    }
}

void Program::use(){
    glUseProgram(_program);
}


//void Shader::set_texture(const char *name, int uniform_location)
//{

//}


Program::~Program(){
    _delete();
}

