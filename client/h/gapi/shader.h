#pragma once

class Shader {
public:
    enum class Type { VERTEX_SHADER=GL_VERTEX_SHADER, FRAGMENT_SHADER=GL_FRAGMENT_SHADER};
private:
    friend class Program;

    uint _shader = 0;
    Type _type;
public:

    Shader(){}
    Shader(const Shader &that) = delete;
    Shader(Shader &&shader);

    void delete_();

    /** Safe VBO removal. Should be used only internally. A class user should just use destructor. */
    void _delete();

    void compile(Type type, const char *source);

    ~Shader();
};
