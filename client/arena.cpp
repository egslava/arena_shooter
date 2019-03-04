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

#include "gapi/gapi.h"
#include "scene/camera.h"
#include "scene/scene.h"
//#include <GL/glu.h>

#include <math.h>
#include "math/math.h"

#include "game/bullet.h"

#define SDL_SAFE(code) do { \
    int result = code;\
    if (result == -1){ \
        throw MySDLException(#code);\
    } \
}while(false);

#include "game/level.h"

class MySDLApp {
    SDL_Window* _window;
    const int viewport_width = 640*1.5;
    const int viewport_height = 480*1.5;

    SDL_GLContext _gl_context;
    Level level;

    void _print_sys_info(){
        int major_version = 0;
        int minor_version = 0;
        SDL_SAFE(SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &major_version));
        SDL_SAFE(SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &minor_version));

        printf("The OpenGL version available is: %d.%d\n", major_version, minor_version);
    }

    bool keys_pressed[SDL_NUM_SCANCODES];
    bool is_shift_pressed = false;
    bool is_ctrl_pressed = false;
public:

    MySDLApp(){
        for (int i = 0; i < SDL_NUM_SCANCODES; i++) keys_pressed[i] = false;

        if (SDL_Init(SDL_INIT_VIDEO) < 0){
            throw MySDLException("Can not init SDL");
        }

        SDL_SAFE(SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1));
        SDL_SAFE(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4));
        SDL_SAFE(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2));
        SDL_SAFE(SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE));
//        SDL_SAFE(SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16));
        SDL_SAFE(SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1));
//        SDL_SAFE(SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8));
//        SDL_SAFE(SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16));
//        SDL_SAFE(SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32));



        _window=SDL_CreateWindow("Hello", 1, 1, viewport_width, viewport_height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

        if (!_window){
            throw MySDLException("Can not create window!");
        }
        SDL_ShowWindow(_window);

        _gl_context = SDL_GL_CreateContext(_window);

        if (!_gl_context){
            throw MySDLException("Can not create an OpenGL context!");
        }
        SDL_SAFE(SDL_GL_SetSwapInterval(1));  // vsync
        SDL_SAFE(SDL_ShowCursor(SDL_DISABLE));  // vsync

        glewInit();

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable( GL_LINE_SMOOTH );
        glEnable(GL_MULTISAMPLE);


        glViewport(0, 0, viewport_width, viewport_height);

        GLdouble os = 10;   // ortho_size
        _print_sys_info();
        glEnable(GL_DEPTH_TEST);
        level.init();
        level.player->camera.turn_up(0.5 * M_PI);
        level.player->camera.fly(1.001);  // TODO: remove this ducktape
        level.player->camera.turn_up(-1. * M_PI);

        //        player->camera._pos = Vec3(4.22, 3.00, 2.17);
        level.player->camera._pos = Vec3(3.99, 12.37, 11.91);
        level.player->camera.rgOX = -0.69;
        level.player->camera.rgOY =  0.03;
    }

    float angleOY = 0.0;

    void loop(){
        bool quit = false;
        SDL_Event event;

        FPSCounter fps_counter;



        float s=1.0;


        using timer=std::chrono::high_resolution_clock ;
        timer::time_point frame_start = timer::now();
        double tick_time = 0;
        while(!quit){
            tick_time = static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(timer::now() - frame_start).count()) / 1000000000.0 ;
            frame_start = timer::now();

            fps_counter.begin();


            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//            models[i_active_level].draw();
//            level.scene._gravity_pass();
//            level.scene._move_colliding();
            level.bullets.update();
            level.scene.integrate();
            level.scene.render();


            // camera navigation
            {
                Vec3 dir;
                float moving_speed = tick_time * (is_ctrl_pressed?6*2:6);
                if (keys_pressed[SDL_SCANCODE_W]){
                    dir += Vec3(0, 0, -moving_speed);
                    level.player->camera.go(moving_speed);
                }
                if (keys_pressed[SDL_SCANCODE_S]){
                    dir += Vec3(0, 0, moving_speed);
                    level.player->camera.go(-moving_speed);
                }
                if (keys_pressed[SDL_SCANCODE_A]){
                    dir += Vec3(-moving_speed, 0, 0);
                    level.player->camera.stride(-moving_speed);
                }
                if (keys_pressed[SDL_SCANCODE_D]){
                    dir += Vec3(moving_speed, 0, 0);
                    level.player->camera.stride(moving_speed);
                }

                if (keys_pressed[SDL_SCANCODE_UP]){
                    dir += Vec3(0, 0, -moving_speed);
                    level.enemy->camera.go(moving_speed);
//                    level.fireball->camera.go(moving_speed);
                }
                if (keys_pressed[SDL_SCANCODE_DOWN]){
                    dir += Vec3(0, 0, moving_speed);
                    level.enemy->camera.go(-moving_speed);
//                    level.fireball->camera.go(-moving_speed);
                }
                if (keys_pressed[SDL_SCANCODE_LEFT]){
                    dir += Vec3(-moving_speed, 0, 0);
                    level.enemy->camera.stride(-moving_speed);
//                    level.fireball->camera.stride(-moving_speed);
                }
                if (keys_pressed[SDL_SCANCODE_RIGHT]){
                    dir += Vec3(moving_speed, 0, 0);
                    level.enemy->camera.stride(moving_speed);
//                    level.fireball->camera.stride(moving_speed);
                }

                if (keys_pressed[SDL_SCANCODE_SPACE]){
                    if (level.player->_on_ground) {
                        level.player->g_velocity = 10;
                        level.player->_on_ground = false;
                    }
                }

                level.nebula->camera.rgOX += 10.0f;
                level.nebula->camera.rgOY += 10.0f;
//                 printf("(%0.2f, %0.2f, %0.2f), %0.2f %0.2f\n", level.player->camera._pos._x, level.player->camera._pos._y, level.player->camera._pos._z, level.player->camera.rgOX, level.player->camera.rgOY);

//                res *= camera.getMatWorldToCamera();
//                camera._pos += res;

            }
            // --- camera navigation

            glEnable(GL_CULL_FACE);
            glFrontFace(GL_CCW);
            glCullFace(GL_BACK);

            fps_counter.end();
            SDL_GL_SwapWindow(_window);
//            SDL_Delay(12);

//            SDL_SetCursor()

            while( SDL_PollEvent( &event ) ){

                if (event.type == SDL_MOUSEBUTTONDOWN){
                    level.bullets.fire(level.player->camera);
                }
                if (event.type == SDL_KEYDOWN){
                    keys_pressed[event.key.keysym.scancode] = true;

                    if (event.key.keysym.sym == SDLK_ESCAPE){
                        return;
                    } else if (event.key.keysym.scancode == SDL_SCANCODE_Z) {
                        bool is_wireframe = level.scene.wireframe();
                        if (is_wireframe){
                            level.scene.ambient_color = Vec3(0, 0, 0, 0);
                        } else {
                            level.scene.ambient_color = Vec3(1,1,1, 1).bright_rgb(0.33);
                        }
                        level.scene.wireframe(!is_wireframe);
                    } else if (event.key.keysym.scancode == SDL_SCANCODE_R){
//                        level.explosion->particles.explode();
//                        level.smoke->particles.explode();

                    } else if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {

                    }
                    is_shift_pressed = event.key.keysym.mod & KMOD_SHIFT;
                    is_ctrl_pressed = event.key.keysym.mod & KMOD_CTRL;

                }

                if (event.type == SDL_KEYUP){
                    keys_pressed[event.key.keysym.scancode] = false;
                }

                if (event.type == SDL_MOUSEMOTION){
                    Sint32 x = event.motion.x, y = event.motion.y;

                    float deltaOY = static_cast<float>(event.motion.xrel) * 0.01;
                    float deltaOX = static_cast<float>(event.motion.yrel) * 0.01;

//                    printf("Mouse delta: %0.2f, %0.2f\n", deltaOX*100., deltaOY*100.);
                    fflush(stdout);
                    level.player->camera.turn_left(-deltaOY);
                    level.player->camera.turn_up(-deltaOX);

//                    angleOY += ;
//                    printf("xrel: %3d, yrel: %3d\n", event.motion.xrel, event.motion.yrel);
                    if (x < viewport_width * 0.1 || x > viewport_width * 0.9 || y < viewport_height * 0.1 || y > viewport_height * 0.9 ){
                        SDL_WarpMouseInWindow(_window, viewport_width / 2, viewport_height / 2);
                        while(SDL_PollEvent( &event ));

                    }
                }

                if (event.type == SDL_MOUSEWHEEL){
                    s += event.wheel.y / 10.f;
                    printf("wheel_y: %3d\n", event.wheel.y);
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

//#define SDL_MAIN_HANDLED

int main (int argc, char *argv[]){

#ifndef NDEBUG
    printf("It's a debug build\n");
#endif

#ifdef NDEBUG
    printf("It's a release build\n");
#endif
    std::cout << GetCurrentWorkingDir() << std::endl;
	printf("Hello world! Version is: %i.%i \n", Arena_VERSION_MAJOR, Arena_VERSION_MINOR);

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

	return 0;
}
