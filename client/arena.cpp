#include <stdio.h>
#include "Config.h"

#define GL_GLEXT_PROTOTYPES
#define GL3_PROTOTYPES

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glext.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "MathFunctions/mysqrt.h"

#ifdef USE_MYMATH
#include "mysqrt.h"
//#include "Table.h"
#endif

//#include <glvnd/>


#include <string>
#include <memory>
#include <exception>
#include <vector>

struct MyException : public std::exception{
    std::string _description;
//    MyException(const char *description):description(description){}
    MyException(std::string description):_description(description) {}

//    virtual operator const char *() const {
//        return description;
//    }
    virtual const char* what() const noexcept{
        return _description.c_str();
    }

//    ~MyException(){}
};

struct MyIllegalStateException : public MyException {
    MyIllegalStateException (const char *description): MyException (description){}
};

struct MySDLException : public MyException {
    MySDLException(const char *description):MyException(description){}
};

struct MyGlException: public MyException {
    MyGlException(const char *description):MyException(description){}
};

struct MyShaderException: public MyGlException {
    MyShaderException(const char *description):MyGlException(description){}
};


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
    Shader(Shader &&shader){
        _shader = shader._shader;
        _type = shader._type;
        shader._shader = 0;
    }

    void delete_(){
        #ifndef NDEBUG
        if (_shader == 0){
            throw MyIllegalStateException("An attempt to delete Shader without calling glCreateShader first.");
        }
        #endif

        glDeleteShader(_shader);
        _shader = 0;
    }

    /** Safe VBO removal. Should be used only internally. A class user should just use destructor. */
    void _delete(){
        if(_shader != 0) delete_();
    }

    void compile(Type type, const char *source){
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

//    void bind(){
//        glBindBuffer(GL_ARRAY_BUFFER, _shader);
//    }

    ~Shader(){
        _delete();
    }
};


class Program {
    uint _program = 0;
    std::vector<Shader> _shaders;   // will be removed automatically, after the destructor is called
public:
    Program(){}
    Program(const Program &that) = delete;
    Program(Program &&that){
        _program = that._program;
        _shaders = std::move(that._shaders);
        that._program = 0;
    }

    void delete_(){
        #ifndef NDEBUG
        if (_program == 0){
            throw MyIllegalStateException("An attempt to delete a Program without calling glCreateProgram() first.");
        }
        #endif

        _shaders.clear();  // call the destructors and remove all the shaders

        glDeleteProgram(_program);
        _program = 0;
    }

    /** Safe Program removal. Should be used only internally. A class user should just use destructor. */
    void _delete(){
        if(_program != 0) delete_();
    }

    void link(Shader &&vertex, Shader &&fragment){
        if (_program != 0) delete_();

        _shaders.push_back(std::move(vertex));
        _shaders.push_back(std::move(fragment));

        _program = glCreateProgram();
        if (_program == 0){
            throw MyGlException("Can't create a Program ");
        }

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

    void use(){
        glUseProgram(_program);
    }

//    void bind(){
//        glBindBuffer(GL_ARRAY_BUFFER, _shader);
//    }

    ~Program(){
        _delete();
    }
};


class VBO {
    GLuint _vbo = 0;

    void _gen(){
        if (_vbo != 0) delete_();
        fprintf(stdout, "glGenBuffers()\n");
        fflush(stdout);
        glGenBuffers(1, &_vbo);
    }

    /** Safe VBO removal. Should be used only internally. A class user should just use destructor. */
    void _delete(){
        if(_vbo != 0) delete_();
    }

    void delete_(){
        #ifndef NDEBUG
        if (_vbo == 0){
            throw MyIllegalStateException("An attempt to delete VBO without Gen it first has been detected.");
        }
        #endif

        glDeleteBuffers(1, &_vbo);
        fprintf(stdout, "glDeleteBuffers()\n");
        fflush(stdout);
        _vbo = 0;
    }
public:
    VBO(){}

    VBO(const VBO &vbo) = delete;
    VBO(VBO &&that){
        _vbo = that._vbo;
        that._vbo = 0;
    }

    VBO&& data(const std::vector<float> data){
        if (_vbo == 0) _gen();

        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_DYNAMIC_DRAW);

        return (VBO&&)*this;
    }

    void bind(){
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    }

    ~VBO(){
        _delete();
    }
};


class VAO {
    GLuint _vao = 0;
    std::vector<VBO> _vbos;

    void _gen(){
        if (_vao != 0) delete_();
        glGenVertexArrays(1, &_vao);
    }

    void delete_(){
        #ifndef NDEBUG
        if (_vao == 0){
            throw MyIllegalStateException("Deletion VAO without a call to glGenVertexArrays.");
        }
        #endif

        _vbos.clear();
        glDeleteVertexArrays(1, &_vao);
        _vao = 0;
    }

    /** Safe VAO removal. Should be used only internally. A class user should just use destructor. */
    void _delete(){
        if(_vao != 0) delete_();
    }

public:
    VAO(){}
    VAO(const VAO &that)=delete;
    VAO(VAO &&that){
        _vao = that._vao;
        _vbos = std::move(that._vbos);
        that._vao = 0;
    }

    VAO&& data(VBO  &&data){
//        if (_vao != 0) delete_();
//        glGenVertexArrays(1, &_vao);
        if (_vao == 0) _gen();
        _vbos.clear();
        _vbos.push_back(std::move(data));
        glBindVertexArray(_vao);

        int counter = 0;
        for (auto &vbo : _vbos){
            vbo.bind();
            glVertexAttribPointer(counter, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

//            typedef void (GLAPIENTRY * PFNGLENABLEVERTEXARRAYATTRIBPROC) (GLuint vaobj, GLuint index);
            glEnableVertexAttribArray(counter);
            counter ++;
        }

        return (VAO&&)*this;
//        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data, GL_STATIC_DRAW);
    }

    void bind(){
        glBindVertexArray(_vao);
    }

    ~VAO(){
        _delete();
    }
};


#define SDL_SAFE(code) do { \
    int result = code;\
    if (result == -1){ \
        throw MySDLException(#code);\
    } \
}while(false);

class MySDLApp {
    SDL_Window* _window;
    SDL_GLContext _gl_context;

    void _print_sys_info(){
        int major_version = 0;
        int minor_version = 0;
        SDL_SAFE(SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &major_version));
        SDL_SAFE(SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &minor_version));

        printf("The OpenGL version available is: %d.%d\n", major_version, minor_version);
    }
public:
    const char *vertex_shader_code = ""
    "#version 330 core 										\n"
    "layout (location = 0) in vec3 aPos;					\n"
    "void main() 											\n"
    "{ 														\n"
    "    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);	\n"
    "}														\n";

    const char *fragment_shader_code = ""
    "#version 330 core                                      \n"
    "out vec4 FragColor;                                    \n"
    "                                                       \n"
    "void main()                                            \n"
    "{                                                      \n"
    "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);          \n"
    "}                                                      \n";

    Program program;

    std::vector<VAO> vaos;
    int i_active_vao = 0;
    VBO vbo_positions;

    MySDLApp(){
        if (SDL_Init(SDL_INIT_VIDEO) < 0){
            throw MySDLException("Can not init SDL");
        }

        SDL_SAFE(SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1));
        SDL_SAFE(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4));
        SDL_SAFE(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2));
        SDL_SAFE(SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE));
        SDL_SAFE(SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1));
//        SDL_SAFE(SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24));

//        mainwindow = SDL_CreateWindow(PROGRAM_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
//                      512, 512, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
//        if (!mainwindow){ /* Die if creation failed */
        _window=SDL_CreateWindow("Hello", 1, 1, 640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

        if (!_window){
            throw MySDLException("Can not create window!");
        }
        SDL_ShowWindow(_window);

        _gl_context = SDL_GL_CreateContext(_window);

        if (!_gl_context){
            throw MySDLException("Can not create an OpenGL context!");
        }
        SDL_SAFE(SDL_GL_SetSwapInterval(1));

        glewInit();

//        glViewport(0, 0, 640, 480);
        _print_sys_info();

//        if (_surface = )

        std::vector<float> _triangle_points {
           0.0f,  0.5f,  0.0f,
           0.5f, -0.5f,  0.0f,
          -0.5f, -0.5f,  0.0f
        };

        std::vector<float> _square_points {
            -0.5f,  -0.5f,  0.0f,
            -0.5f,   0.5f,  0.0f,
             0.5f,   0.5f,  0.0f,

            -0.5f,  -0.5f,  0.0f,
            0.5f,   0.5f,  0.0f,
            0.5f, -0.5f,  0.0f,
        };

        vaos.push_back(VAO().data(VBO().data(_triangle_points)));
        vaos.push_back(VAO().data(VBO().data(_square_points)));

        Shader vertex_shader, fragment_shader;
        vertex_shader.compile(Shader::Type::VERTEX_SHADER, vertex_shader_code);
        fragment_shader.compile(Shader::Type::FRAGMENT_SHADER, fragment_shader_code);

        program.link(std::move(vertex_shader), std::move(fragment_shader));
    }

    void render(){
        bool quit = false;
        SDL_Event event;
        while(!quit){

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            program.use();
            vaos[i_active_vao].bind();
            glDrawArrays(GL_TRIANGLES, 0, 6);

            SDL_GL_SwapWindow(_window);

            while( SDL_PollEvent( &event ) ){
                if (event.type == SDL_KEYDOWN){
                    if (event.key.keysym.sym == SDLK_ESCAPE){
                        return;
                    } else if (event.key.keysym.sym == SDLK_1){
                        i_active_vao = 0;
                    }else if (event.key.keysym.sym == SDLK_2){
                        i_active_vao = 1;
                    }
                }

                if( event.type == SDL_QUIT ){
                    quit = true;
                }
            }
        }
//        SDL_GL_SwapWindow(_window);
    }

    ~MySDLApp(){
        SDL_GL_DeleteContext(_gl_context);
        SDL_DestroyWindow(_window);
        SDL_Quit();
    }
};

////#include <glob.h>
//struct GLFWApp {
//    GLFWApp(){

//    }
//}

#include <iostream>

int main (int argc, const char **argv){
	printf("Hello world! Version is: %i.%i \n", Arena_VERSION_MAJOR, Arena_VERSION_MINOR);

#ifdef USE_MYMATH
    printf("The square of 4: %.2f\n", MySqrt(16));
#endif

#ifdef HAVE_LOG
    printf("The LOG function has been found.\n");
#endif

    try {
        MySDLApp().render();
    } catch (MyException &e) {
        std::cout << e.what() << std::endl;
        throw;
        return 1;
    }



	return 0;
}
