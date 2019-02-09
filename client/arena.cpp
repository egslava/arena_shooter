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
#include "scene/camera.h"
#include "scene/model.h"
//#include <GL/glu.h>

#include <math.h>

#define SDL_SAFE(code) do { \
    int result = code;\
    if (result == -1){ \
        throw MySDLException(#code);\
    } \
}while(false);

class MySDLApp {
    SDL_Window* _window;
    const int viewport_width = 640;
    const int viewport_height = 480;

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

    std::vector<Model> models;
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



        _window=SDL_CreateWindow("Hello", 1, 1, viewport_width, viewport_height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

        if (!_window){
            throw MySDLException("Can not create window!");
        }
        SDL_ShowWindow(_window);

        _gl_context = SDL_GL_CreateContext(_window);

        if (!_gl_context){
            throw MySDLException("Can not create an OpenGL context!");
        }
        SDL_SAFE(SDL_GL_SetSwapInterval(0));  // vsync
        SDL_SAFE(SDL_ShowCursor(SDL_DISABLE));  // vsync

        glewInit();

        glEnable(GL_DEPTH_TEST);

//        gluPerspective ( 90, (GLint)width/ (GLint)height, 0.0, 200.0 );

//        glMatrixMode( GL_MODELVIEW );
//        glLoadIdentity();
        glEnable (GL_DEPTH_TEST);

        glViewport(0, 0, viewport_width, viewport_height);
//        glDepthRange(-100, 100);

        GLdouble os = 10;   // ortho_size
        glOrtho(-os, os, -os, os, -os, os);
        _print_sys_info();

//        if (_surface = )


        models.push_back(Model().load("./res/plane.model"));
//        vaos.push_back(VAO().data(VBO().data(_triangle_points), VBO().data(_triangle_colors), VBO().data(_triangle_texcoords, 2)));
        models.push_back(Model().load("./res/warehouse.model"));
//        models.push_back(Model(
//                             VAO().data(VBO().data(_square_points), VBO().data(_square_colors), VBO().data(_square_texcoords, 2))));
        models.push_back(Model().load("./res/cube.model"));
        models.push_back(Model().load("./res/strange.model"));
        models.push_back(Model().load("./res/axes.model"));
        models.push_back(Model().load("./res/axes-cube.model"));


        Shader vertex_shader, fragment_shader;
        vertex_shader.compile(Shader::Type::VERTEX_SHADER, vertex_shader_code);
        fragment_shader.compile(Shader::Type::FRAGMENT_SHADER, fragment_shader_code);

        program.link(std::move(vertex_shader), std::move(fragment_shader));
//        texture.data("../client/res/texture.png");

        textures.push_back(Texture().data("./res/cottage.pvr"));
        textures.push_back(Texture().data("./res/cottage.pvr"));
        textures.push_back(Texture().data("./res/pvr_tex_tool_icon.pvr"));
//        textures.push_back(Texture().data("./res/cottage.pvr"));
//        textures.push_back(Texture().data("./res/pvr_tex_tool_icon.pvr"));
        textures.push_back(Texture().data("./res/cottage.pvr"));
        textures.push_back(Texture().data("./res/cottage.pvr"));
        textures.push_back(Texture().data("./res/axes.pvr"));
        textures.push_back(Texture().data("./res/axes-cube.pvr"));
//        textures.push_back(Texture().data("./res/pvr_tex_tool_icon.pvr"));
    }

    uint vbo_random_points, vao_random_points;
    uint current_point;

    float angleOY = 0.0;

    void loop(){
        bool quit = false;
        SDL_Event event;
        int i_stride_vertex = 0;

        FPSCounter fps_counter;


        bool keys_pressed[SDL_NUM_SCANCODES];
        for (int i = 0; i < SDL_NUM_SCANCODES; i++) keys_pressed[i] = false;

        bool is_left_pressed = false;
        bool is_right_pressed = false;
        float x=0, y=0, z=0, s=1.0;

        Camera camera;

        using timer=std::chrono::high_resolution_clock ;
        timer::time_point frame_start = timer::now();
        double tick_time = 0;
        while(!quit){
            tick_time = static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(timer::now() - frame_start).count()) / 1000000000.0 ;
            frame_start = timer::now();

            fps_counter.measure();

            glEnable(GL_DEPTH_TEST);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            models[i_active_vao].draw();
            textures[i_active_vao].bind();

//            camera.turn_left(-tick_time);
//            camera.turn_up(tick_time/3.);

            // camera navigation
            {
                Vec3 dir;
                float moving_speed = 1 * tick_time;
                if (keys_pressed[SDL_SCANCODE_W]){
                    dir += Vec3(0, 0, -moving_speed);
                    // camera.go(moving_speed);
                }
                if (keys_pressed[SDL_SCANCODE_S]){
                    dir += Vec3(0, 0, moving_speed);
//                    camera.go(-moving_speed);
                }
                if (keys_pressed[SDL_SCANCODE_A]){
                    dir += Vec3(-moving_speed, 0, 0);
//                    camera.stride(-moving_speed);
                }
                if (keys_pressed[SDL_SCANCODE_D]){
                    dir += Vec3(moving_speed, 0, 0);
//                    camera.stride(moving_speed);
                }


//                Vec3 cam = camera.getMatCameraToWorld() * Vec3(0, 0, 0);
//                Vec3 res;
//                Line l (cam,
//                        camera.getMatCameraToWorld() * Vec3(0, 0, -1));


//                dir = camera.getMatCameraToWorld() * dir;

                Vec3 cam = camera.getMatCameraToWorld() * Vec3(0, 0, 0);
                dir = camera.getMatCameraToWorld() * dir;
                Vec3 res;
                Line l (cam,
                        dir);



                float nearest_distance = 10000;
                float min_distance = 0.075;
//                for (const Triangle &tri: models[i_active_vao]._triangles){
//                    LinePlaneIntersectionResult p=intersection(l, tri);
//                    if (p.state != p.State::ONE) continue;

////                    printf("Bump!\n");

////                    float d = (cam - p.pos).len3();
////                    float d = static_cast<Plane>(tri).distance_to(camera._pos);
//                    float d = static_cast<Plane>(tri).distance_to(dir);
//                    float d2 = static_cast<Plane>(tri).distance_to(cam);
//                    // printf("line: c: %f;%f;%f, s: %f;%f;%f\n", l.c._x, l.c._y, l.c._z, l.s._x, l.s._y, l.s._z);
//                    // printf("intersection. x: %f, y: %f, z: %f\n", p.pos._x, p.pos._y, p.pos._z);
//                    // printf("triangle. %0.2f,%0.2f,%0.2f;  %0.2f,%0.2f,%0.2f;  %0.2f,%0.2f,%0.2f\n", tri.A._x, tri.A._y, tri.A._z, tri.B._x, tri.B._y, tri.B._z, tri.C._x, tri.C._y, tri.C._z);

//                    if (fabs(d) <= min_distance ){ //&& fabs(d) <= nearest_distance){
//                        nearest_distance = fabs(d);
////                        res = tri.n() * dir.len3();
////                        res = tri.n() * (tri.n().dot3(dir));
////                        res += tri.n() * (0.15f-fabs(d)) * (d<0?-1:1);
//                        res = (d<0?-1:1)*tri.n() * (min_distance-fabs(d));
//                    }
//                }

                cam = dir + res;
                camera._pos = cam; //dir + res;


                for (const Triangle &tri: models[i_active_vao]._triangles){
                    Line l (cam, cam+tri.n());
                    LinePlaneIntersectionResult p=intersection(l, tri);
                    if (p.state != p.State::ONE) continue;

                    float d = static_cast<Plane>(tri).distance_to(cam);
                    float d2 = static_cast<Plane>(tri).distance_to(cam);
                    // printf("line: c: %f;%f;%f, s: %f;%f;%f\n", l.c._x, l.c._y, l.c._z, l.s._x, l.s._y, l.s._z);
                    // printf("intersection. x: %f, y: %f, z: %f\n", p.pos._x, p.pos._y, p.pos._z);
                    // printf("triangle. %0.2f,%0.2f,%0.2f;  %0.2f,%0.2f,%0.2f;  %0.2f,%0.2f,%0.2f\n", tri.A._x, tri.A._y, tri.A._z, tri.B._x, tri.B._y, tri.B._z, tri.C._x, tri.C._y, tri.C._z);
                    if (fabs(d) <= min_distance ){ //&& fabs(d) <= nearest_distance){
                        nearest_distance = fabs(d);
//                        res = tri.n() * dir.len3();
//                        res = tri.n() * (tri.n().dot3(dir));
//                        res += tri.n() * (0.15f-fabs(d)) * (d<0?-1:1);
                        res += (d<0?-1:1)*tri.n() * (min_distance-fabs(d));
                    }
                }

                cam = cam + res;
                camera._pos = cam; //dir + res;

//                line: c: 0.000080;0.012763;-0.000100, s: -0.006794;0.451772;1.043850
//                intersection. x: -0.006423, y: 0.511282, z: 1.043385
//                triangle. -0.00,0.51,0.94;  -0.00,0.51,-0.85;  0.50,0.18,-0.85
//                if (nearest_distance < 10000){
//                    printf("Neares distance: %.4f\n", nearest_distance);
//                }


//                res *= camera.getMatWorldToCamera();
//                camera._pos += res;

            }
            // --- camera navigation


            if (is_left_pressed){
                angleOY -= 0.001;
            }
            if (is_right_pressed){
                angleOY += 0.001;
            }

            program.transform(angleOY, x, y, z, s, s, s);
            program.use(camera);

//            glDrawArrays(GL_TRIANGLES, 0, 3600);

            glDisable(GL_DEPTH_TEST);
            SDL_GL_SwapWindow(_window);
            SDL_Delay(12);

//            SDL_SetCursor()

            bool skip_warp = 0;
            while( SDL_PollEvent( &event ) ){

                if (event.type == SDL_KEYDOWN){
                    keys_pressed[event.key.keysym.scancode] = true;

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
                    } else if (event.key.keysym.sym == SDLK_5){
                        i_active_vao = 4;
                    } else if (event.key.keysym.sym == SDLK_6){
                        i_active_vao = 5;
                    } else if (event.key.keysym.sym == SDLK_7){
                        i_active_vao = 6;
                    } else if (event.key.keysym.sym == SDLK_RIGHT){
                        is_right_pressed = true;
                    } else if (event.key.keysym.sym == SDLK_LEFT){
                        is_left_pressed = true;
                    }
                }

                if (event.type == SDL_KEYUP){
                    keys_pressed[event.key.keysym.scancode] = false;

                    if (event.key.keysym.sym == SDLK_RIGHT){
                        is_right_pressed = false;
                    } else if (event.key.keysym.sym == SDLK_LEFT){
                        is_left_pressed = false;
                    }
                }


                if (event.type == SDL_MOUSEMOTION){
                    if (skip_warp > 0){
                        skip_warp -= 1;
                        continue;
                    }
                    Sint32 x = event.motion.x, y = event.motion.y;

                    float deltaOY = static_cast<float>(event.motion.xrel) * 0.01;
                    float deltaOX = static_cast<float>(event.motion.yrel) * 0.01;

//                    printf("Mouse delta: %0.2f, %0.2f\n", deltaOX*100., deltaOY*100.);
                    fflush(stdout);
                    camera.turn_left(-deltaOY);
                    camera.turn_up(-deltaOX);

//                    angleOY += ;
//                    printf("xrel: %3d, yrel: %3d\n", event.motion.xrel, event.motion.yrel);
                    if (x < viewport_width * 0.1 || x > viewport_width * 0.9 || y < viewport_height * 0.1 || y > viewport_height * 0.9 ){
//                        skip_warp += 20;
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

#include "math/matrix.h"
int main (int argc, char *argv[]){

//    Mat4x4 i1 = Mat4x4::I;
//    Mat4x4 i2 = Mat4x4::I;
//    Mat4x4 i3 = i1 * i2;

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
