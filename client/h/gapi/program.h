#pragma once

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

    void use();

//    void bind(){
//        glBindBuffer(GL_ARRAY_BUFFER, _shader);
//    }
//    void set_texture(const char *name, int uniform_location);

    ~Program();
};
