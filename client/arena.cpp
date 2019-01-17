#include <stdio.h>
#include "Config.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "MathFunctions/mysqrt.h"

#ifdef USE_MYMATH
#include "mysqrt.h"
//#include "Table.h"
#endif

//#include <glvnd/>
#include <GL/gl.h>
#include <memory>
#include <exception>

struct MyException {
    const char *description;
    MyException(const char *description):description(description){}

    operator const char *() const {
        return description;
    }
    const char* what() const noexcept{
        return this->description;
    }
};

struct MySDLException : public MyException {
    MySDLException(const char *description):MyException(description){}
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
    MySDLApp(){
        if (SDL_Init(SDL_INIT_VIDEO) < 0){
            throw MySDLException("Can not init SDL");
        }

        SDL_SAFE(SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1));
        SDL_SAFE(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4));
        SDL_SAFE(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2));
        SDL_SAFE(SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE));
        SDL_SAFE(SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1));
        SDL_SAFE(SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24));

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

        _print_sys_info();

//        if (_surface = )
    }

    void render(){
        glClearColor(0,0,0,0);


//        glClear(GL_COLOR_BUFFER_BIT);
        bool quit = false;
        SDL_Event event;
        while(!quit){

//          Display();
          SDL_GL_SwapWindow(_window);

          while( SDL_PollEvent( &event ) ){
              if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE){
                  return;
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

        fprintf(stderr, e.what());
        return 1;
    }




	return 0;
}
