#include <stdio.h>
#include "Config.h"

#define GL_GLEXT_PROTOTYPES
#define GL3_PROTOTYPES


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

#include "gapi.h"
#include "resources.h"

//#include <GL/glu.h>

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

    Program program;

    std::vector<VAO> vaos;
    int i_active_vao = 0;
    VBO vbo_positions;
    std::vector<Texture> textures;

    MySDLApp(){
        if (SDL_Init(SDL_INIT_VIDEO) < 0){
            throw MySDLException("Can not init SDL");
        }

        SDL_SAFE(SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1));
        SDL_SAFE(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4));
        SDL_SAFE(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2));
        SDL_SAFE(SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE));
//        SDL_SAFE(SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16));
        SDL_SAFE(SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1));
//        SDL_SAFE(SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16));
//        SDL_SAFE(SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32));



        _window=SDL_CreateWindow("Hello", 1, 1, 600, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

        if (!_window){
            throw MySDLException("Can not create window!");
        }
        SDL_ShowWindow(_window);

        _gl_context = SDL_GL_CreateContext(_window);

        if (!_gl_context){
            throw MySDLException("Can not create an OpenGL context!");
        }
        SDL_SAFE(SDL_GL_SetSwapInterval(0));  // vsync

        glewInit();

        glEnable(GL_DEPTH_TEST);

//        gluPerspective ( 90, (GLint)width/ (GLint)height, 0.0, 200.0 );

        glMatrixMode( GL_MODELVIEW );
        glLoadIdentity();
        glEnable (GL_DEPTH_TEST);

//        glViewport(0, 0, 640, 480);
        _print_sys_info();

//        if (_surface = )


        vaos.push_back(VAO().load("./res/plane.model"));
//        vaos.push_back(VAO().data(VBO().data(_triangle_points), VBO().data(_triangle_colors), VBO().data(_triangle_texcoords, 2)));
        vaos.push_back(VAO().load("./res/warehouse.model"));
        vaos.push_back(VAO().data(VBO().data(_square_points), VBO().data(_square_colors), VBO().data(_square_texcoords, 2)));
        vaos.push_back(VAO().load("./res/cube.model"));


        Shader vertex_shader, fragment_shader;
        vertex_shader.compile(Shader::Type::VERTEX_SHADER, vertex_shader_code);
        fragment_shader.compile(Shader::Type::FRAGMENT_SHADER, fragment_shader_code);

        program.link(std::move(vertex_shader), std::move(fragment_shader));
//        texture.data("../client/res/texture.png");

        textures.push_back(Texture().data("./res/cottage.pvr"));
        textures.push_back(Texture().data("./res/cottage.pvr"));
//        textures.push_back(Texture().data("./res/pvr_tex_tool_icon.pvr"));
        textures.push_back(Texture().data("./res/cottage.pvr"));
//        textures.push_back(Texture().data("./res/pvr_tex_tool_icon.pvr"));
        textures.push_back(Texture().data("./res/cottage.pvr"));
//        textures.push_back(Texture().data("./res/pvr_tex_tool_icon.pvr"));
    }

    uint vbo_random_points, vao_random_points;
    uint current_point;

    void loop(){
        bool quit = false;
        SDL_Event event;
        int i_stride_vertex = 0;

        FPSCounter fps_counter;

        while(!quit){
            fps_counter.measure();

            glEnable(GL_DEPTH_TEST);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            vaos[i_active_vao].bind();
            textures[i_active_vao].bind();
            program.use();

            glDrawArrays(GL_TRIANGLES, 0, 3600);

            glDisable(GL_DEPTH_TEST);
            SDL_GL_SwapWindow(_window);
//            SDL_Delay(12);
            while( SDL_PollEvent( &event ) ){
                if (event.type == SDL_KEYDOWN){
                    if (event.key.keysym.sym == SDLK_ESCAPE){
                        return;
                    } else if (event.key.keysym.sym == SDLK_1){
                        i_active_vao = 0;
                    } else if (event.key.keysym.sym == SDLK_2){
                        i_active_vao = 1;
                    } else if (event.key.keysym.sym == SDLK_3){
                        i_active_vao = 2;
                    } else if (event.key.keysym.sym == SDLK_4){
                        i_active_vao = 3;
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


#include <iostream>
#include <array>
//void print_numbers(std::vector<int&&> ints){
//    for (auto &i : ints){
//        printf("%d\n", i);
//    }
//}

#include <stdio.h>  /* defines FILENAME_MAX */
#ifdef WIN32
	#define WINDOWS  /* uncomment this line to use it for windows.*/
#endif
#ifdef WINDOWS
	#include <direct.h>
	#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif
#include<iostream>

std::string GetCurrentWorkingDir( void ) {
  char buff[FILENAME_MAX];
  GetCurrentDir( buff, FILENAME_MAX );
  std::string current_working_dir(buff);
  return current_working_dir;
}

#pragma pack(4)
struct BinFile{
    int zero=0, one=1, two=2;
    float f_zero=0, f_one=1, f_two=2;
    int i_zero=0, i_one=1, i_two=2;
};

#include "gapi/loaders/pvr.h"

//#define SDL_MAIN_HANDLED
int main (int argc, char *argv[]){
//    FILE *bin_file = fopen("test.bin", "wb");
//    if(!bin_file) throw MyFileException("Can't open a binary file for writing");

//    BinFile data;
//    fwrite(&data, sizeof(BinFile), 1, bin_file);
//    fclose(bin_file);
//    return 0;
#ifndef NDEBUG
    printf("It's a debug build\n");
#endif

#ifdef NDEBUG
    printf("It's a release build\n");
#endif
//    int width, height, depth, n_channels;
//    pvr_load("../client/res/pvr_tex_tool_icon.pvr", &width, &width, &height);
//    return 0;
    std::cout << GetCurrentWorkingDir() << std::endl;
    printf("test\n");


	printf("Hello world! Version is: %i.%i \n", Arena_VERSION_MAJOR, Arena_VERSION_MINOR);

#ifdef USE_MYMATH
    printf("The square of 4: %.2f\n", MySqrt(16));
#endif

#ifdef HAVE_LOG
    printf("The LOG function has been found.\n");
#endif

    try {
        MySDLApp().loop();
    } catch (MyException &e) {
        std::cout << e.what() << std::endl;
        throw;
        return 1;
    }

//    int &&i = 1, &&j = 2;
//    print_numbers({i, j});


	return 0;
}
