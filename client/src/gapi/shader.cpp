#include "gapi.h"

Shader::Shader(Shader &&shader){
    _shader = shader._shader;
    _type = shader._type;
    shader._shader = 0;
}

void Shader::delete_(){
    #ifndef NDEBUG
        if (_shader == 0){
            throw MyIllegalStateException("An attempt to delete Shader without calling glCreateShader first.");
        }
    #endif

    glDeleteShader(_shader);
    _shader = 0;
}

void Shader::_delete(){
    if(_shader != 0) delete_();
}

void Shader::compile(Shader::Type type, const char *source){
    if (_shader != 0) delete_();
    _shader = glCreateShader( (GLenum)type);
    //        typedef void (GLAPIENTRY * PFNGLSHADERSOURCEPROC) (GLuint shader, GLsizei count, const GLchar *const* string, const GLint* length);

    glShaderSource(_shader, 1, &source, nullptr);
    glCompileShader(_shader);

    int success;
    char compilation_errors[512];
    // typedef void (GLAPIENTRY * PFNGLGETSHADERINFOLOGPROC) (GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
    // typedef void (GLAPIENTRY * PFNGLGETSHADERIVPROC) (GLuint shader, GLenum pname, GLint* param);
    glGetShaderiv(_shader, GL_COMPILE_STATUS, &success);
    if (!success){
        glGetShaderInfoLog(_shader, 512, nullptr, compilation_errors);
        throw MyShaderException(compilation_errors);
    }
}

Shader::~Shader(){
    _delete();
}
