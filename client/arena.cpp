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
#include "scene/camera.h"
#include "scene/model.h"
#include "scene/scene.h"
//#include <GL/glu.h>

#include <math.h>

#define SDL_SAFE(code) do { \
    int result = code;\
    if (result == -1){ \
        throw MySDLException(#code);\
    } \
}while(false);

struct Level {
    Scene scene;
    SPNode player;
    SPNodes enemies;

    void init(){
        scene.init();
        scene.nodes.emplace_back(new Node{"Ground", Node::Flags::NONE, Node::PhysFlags::SOLID, (Model().load("res/level/Ground.model", Texture().data("./res/level/textures/Ground Light Tex.pvr")/*, Color(WHITE)*/))});
//        scene.nodes.emplace_back(new Nododel().load("res/leNode::Flags::RIGID, e{(Mvel1/Ground.model", Texture().data("./res/level/textures/Ground.pvr")/*, Color(WHITE)*/))});
        scene.nodes.emplace_back(new Node{"Nebula", Node::Flags::NONE, Node::PhysFlags::SOLID, (Model().load("res/level/Nebula_Sky.model", Texture().data("./res/level/textures/nebula.pvr")/*, Color(BLACK)*/))});
        scene.nodes.emplace_back(new Node{"Flower", Node::Flags::NONE, Node::PhysFlags::SOLID, (Model().load("res/level/Flower.model", Texture().data("./res/level/textures/flower_lm.pvr")/*, Color(CYAN)*/))});
//        scene.nodes.emplace_back(new Nododel().load("res/leNode::Flags::RIGID, e{(Mvel1/stairs.model", Texture().data("./res/level/textures/stairs.pvr")/*, Color(WHITE)*/))});
        scene.nodes.emplace_back(new Node{"Roof top", Node::Flags::NONE, Node::PhysFlags::SOLID, (Model().load("res/level/RoofTop.model", Texture().data("./res/level/textures/RoofLightMap.pvr")/*, Color(RED/PINK)*/))});
        scene.nodes.emplace_back(new Node{"Crystal", Node::Flags::NONE, Node::PhysFlags::SOLID, (Model().load("res/level/Crystal_001.model", Texture().data("./res/level/textures/color_white.pvr")/*, Color(RED/PINK)*/))});

        player = make_shared<Node>();
        player->name = "Player";
        player->flags = Node::Flags::NONE;
        player->phys = Node::PhysFlags::RIGID;
        scene.nodes.emplace_back(player);
        scene._camera = player;
    }
};


class MySDLApp {
    SDL_Window* _window;
    const int viewport_width = 640*2;
    const int viewport_height = 480*2;

    SDL_GLContext _gl_context;
    Level level;

    void _print_sys_info(){
        int major_version = 0;
        int minor_version = 0;
        SDL_SAFE(SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &major_version));
        SDL_SAFE(SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &minor_version));

        printf("The OpenGL version available is: %d.%d\n", major_version, minor_version);
    }
public:



//    std::vector<Model> models;
//    Model model_environment;
//    int i_active_level = 0;
//    VBO vbo_positions;
//    std::vector<Texture> textures;

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
//        glOrtho(-os, os, -os, os, -os, os);
        _print_sys_info();

//        if (_surface = )


//        models.push_back(Model().load("./res/plane.model", Texture().data("./res/cottage.pvr")));
////        vaos.push_back(VAO().data(VBO().data(_triangle_points), VBO().data(_triangle_colors), VBO().data(_triangle_texcoords, 2)));
//        models.push_back(Model().load("./res/warehouse.model", Texture().data("./res/cottage.pvr")));
////        models.push_back(Model(
////                             VAO().data(VBO().data(_square_points), VBO().data(_square_colors), VBO().data(_square_texcoords, 2))));
//        models.push_back(Model().load("./res/level/Ground.model", Texture().data("./res/level/textures/Ground Light Tex.pvr")));

//        models.push_back(Model().load("./res/lightmaps/room.model", Texture().data("./res/lightmaps/lightmap.pvr")));
//        models.push_back(Model().load("./res/cube.model", Texture().data("./res/pvr_tex_tool_icon.pvr")));
////        models.push_back(Model().load("./res/strange.model", Texture().data("./res/axes.pvr")));
//        models.push_back(Model().load("./res/axes.model", Texture().data("./res/axes.pvr")));
//        models.push_back(Model().load("./res/axes-cube.model", Texture().data("./res/axes-cube.pvr")));
//        models.push_back(Model().load("./res/level/Flower.model", Texture().data("./res/lightmaps/lightmap.pvr")));
////        models.push_back(Model().load("./res/landscape.model", Texture().data("./res/lightmaps/lightmap.pvr")));
//        model_environment.load("./res/lightmaps/environment.model", Texture().data("./res/level/textures/nebula.pvr"));


//        texture.data("../client/res/texture.png");

//        textures.push_back();
//        textures.push_back();
//        textures.push_back();
////        textures.push_back(Texture().data("./res/cottage.pvr"));
////        textures.push_back(Texture().data("./res/pvr_tex_tool_icon.pvr"));
//        textures.push_back(Texture().data("./res/cottage.pvr"));
//        textures.push_back(Texture().data("./res/cottage.pvr"));
//        textures.push_back();
//        textures.push_back();
//        textures.push_back(Texture().data("./res/pvr_tex_tool_icon.pvr"));
        level.init();
    }

//    uint vbo_random_points, vao_random_points;
//    uint current_point;

    float angleOY = 0.0;

    void loop(){
        bool quit = false;
        SDL_Event event;

        FPSCounter fps_counter;


        bool keys_pressed[SDL_NUM_SCANCODES];
        for (int i = 0; i < SDL_NUM_SCANCODES; i++) keys_pressed[i] = false;

        float s=1.0;


        using timer=std::chrono::high_resolution_clock ;
        timer::time_point frame_start = timer::now();
        double tick_time = 0;
        while(!quit){
            tick_time = static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(timer::now() - frame_start).count()) / 1000000000.0 ;
            frame_start = timer::now();

            fps_counter.measure();

            glEnable(GL_DEPTH_TEST);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//            models[i_active_level].draw();
            level.scene.move_colliding();
            level.scene.render();


            // camera navigation
            {
                Vec3 dir;
                float moving_speed = 2 * tick_time;
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

//                res *= camera.getMatWorldToCamera();
//                camera._pos += res;

            }
            // --- camera navigation


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
                    }
//                    else if (event.key.keysym.sym == SDLK_1){
//                        i_active_level = 0;
//                    } else if (event.key.keysym.sym == SDLK_2){
//                        i_active_level = 1;
//                    } else if (event.key.keysym.sym == SDLK_3){
//                        i_active_level = 2;
//                    } else if (event.key.keysym.sym == SDLK_4){
//                        i_active_level = 3;
//                    } else if (event.key.keysym.sym == SDLK_5){
//                        i_active_level = 4;
//                    } else if (event.key.keysym.sym == SDLK_6){
//                        i_active_level = 5;
//                    } else if (event.key.keysym.sym == SDLK_7){
//                        i_active_level = 6;
//                    } else if (event.key.keysym.sym == SDLK_8){
//                        i_active_level = 7;
//                    } else if (event.key.keysym.sym == SDLK_RIGHT){
//                        is_right_pressed = true;
//                    } else if (event.key.keysym.sym == SDLK_LEFT){
//                        is_left_pressed = true;
//                    }
                }

                if (event.type == SDL_KEYUP){
                    keys_pressed[event.key.keysym.scancode] = false;
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
                    level.player->camera.turn_left(-deltaOY);
                    level.player->camera.turn_up(-deltaOX);

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
