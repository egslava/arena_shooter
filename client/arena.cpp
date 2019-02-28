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

#define SDL_SAFE(code) do { \
    int result = code;\
    if (result == -1){ \
        throw MySDLException(#code);\
    } \
}while(false);

struct Level {
    Scene scene;
    SPNode player, enemy, explosion, smoke, fireball, nebula;
    SPNodes enemies;

    void init(){
        scene.init();
        scene.nodes.emplace_back(new Node{"Ground", Node::Flags::NONE, Node::PhysFlags::SOLID, (Model().load("res/level/Ground.model", Texture().data("./res/level/textures/Ground Light Tex2.pvr")/*, Color(WHITE)*/))});
//        scene.nodes.emplace_back(new Nododel().load("res/leNode::Flags::RIGID, e{(Mvel1/Ground.model", Texture().data("./res/level/textures/Ground.pvr")/*, Color(WHITE)*/))});

        nebula = make_shared<Node>();
        nebula->name = "Nebula";
        nebula->phys = Node::PhysFlags::GHOST;
        nebula->flags = Node::Flags::NONE;
        nebula->model = std::move(Model().load("res/level/Nebula_Sky.model", Texture().data("./res/level/textures/nebula.pvr")));
        scene.nodes.emplace_back(nebula);


        scene.nodes.emplace_back(new Node{"Flower", Node::Flags::NONE, Node::PhysFlags::SOLID, (Model().load("res/level/Flower.model", Texture().data("./res/level/textures/flower_lm.pvr")/*, Color(CYAN)*/).color(Vec3(0.063, 0.041, 0.402).bright_rgb(3)))});
//        scene.nodes.emplace_back(new Nododel().load("res/leNode::Flags::RIGID, e{(Mvel1/stairs.model", Texture().data("./res/level/textures/stairs.pvr")/*, Color(WHITE)*/))});
        scene.nodes.emplace_back(new Node{"Roof", Node::Flags::NONE, Node::PhysFlags::SOLID, (Model().load("res/level/Roof.model", Texture().data("./res/level/textures/TableLightMap.pvr")/*, Color(RED/PINK)*/).color(Vec3(0.319, 0, 0.003).bright_rgb(3)))});
        scene.nodes.emplace_back(new Node{"Roof top", Node::Flags::NONE, Node::PhysFlags::SOLID, (Model().load("res/level/RoofTop.model", Texture().data("./res/level/textures/RoofLightMap.pvr")/*, Color(RED/PINK)*/).color(Vec3(0.319, 0, 0.003).bright_rgb(3)))});
        scene.nodes.emplace_back(new Node{"Crystal", Node::Flags::NONE, Node::PhysFlags::SOLID, (Model().load("res/level/Crystal_Big.model", Texture().data("./res/level/textures/color_white.pvr")/*, Color(RED/PINK)*/))});
        scene.nodes.emplace_back(new Node{"Crystal", Node::Flags::NONE, Node::PhysFlags::SOLID, (Model().load("res/level/Crystal_Top.model", Texture().data("./res/level/textures/color_white.pvr")/*, Color(RED/PINK)*/))});
        scene.nodes.emplace_back(new Node{"Crystal", Node::Flags::NONE, Node::PhysFlags::SOLID, (Model().load("res/level/CrystalBottom.model", Texture().data("./res/level/textures/color_white.pvr")/*, Color(RED/PINK)*/))});
        scene.nodes.emplace_back(new Node{"BallLight1", Node::Flags::NONE, Node::PhysFlags::SOLID, (Model().load("res/level/BallLight1.model", Texture().data("./res/level/textures/color_white.pvr")/*, Color(RED/PINK)*/))});
        scene.nodes.emplace_back(new Node{"BallLight2", Node::Flags::NONE, Node::PhysFlags::SOLID, (Model().load("res/level/BallLight2.model", Texture().data("./res/level/textures/color_white.pvr")/*, Color(RED/PINK)*/))});
        scene.nodes.emplace_back(new Node{"BallLight3", Node::Flags::NONE, Node::PhysFlags::SOLID, (Model().load("res/level/BallLight3.model", Texture().data("./res/level/textures/color_white.pvr")/*, Color(RED/PINK)*/))});
        scene.nodes.emplace_back(new Node{"BallLight4", Node::Flags::NONE, Node::PhysFlags::SOLID, (Model().load("res/level/BallLight4.model", Texture().data("./res/level/textures/color_white.pvr")/*, Color(RED/PINK)*/))});

        scene.nodes.emplace_back(new Node{"Stairs", Node::Flags::NONE, Node::PhysFlags::SOLID, (Model().load("res/level/Stairs1.model", Texture().data("./res/level/textures/stairs_lightmap1.pvr")/*, Color(RED/PINK)*/))});
        scene.nodes.emplace_back(new Node{"Stairs", Node::Flags::NONE, Node::PhysFlags::SOLID, (Model().load("res/level/Stairs2.model", Texture().data("./res/level/textures/stairs_lightmap2.pvr")/*, Color(RED/PINK)*/))});
        scene.nodes.emplace_back(new Node{"Stairs", Node::Flags::NONE, Node::PhysFlags::SOLID, (Model().load("res/level/Stairs3.model", Texture().data("./res/level/textures/stairs_lightmap3.pvr")/*, Color(RED/PINK)*/))});
        scene.nodes.emplace_back(new Node{"Stairs", Node::Flags::NONE, Node::PhysFlags::SOLID, (Model().load("res/level/Stairs4.model", Texture().data("./res/level/textures/stairs_lightmap4.pvr")/*, Color(RED/PINK)*/))});
        scene.nodes.emplace_back(new Node{"Sphere", Node::Flags::NONE, Node::PhysFlags::SOLID, (Model().load("res/debug/sphere_r1.model", Texture().data("./res/debug/grid.pvr")/*, Color(RED/PINK)*/))});

        player = make_shared<Node>();
        player->name = "Player";
        player->flags = Node::Flags::NONE;
        player->phys = Node::PhysFlags::RIGID;
//        player->camera._pos = Vec3(0, 2, 0);

        scene.nodes.emplace_back(player);
        scene._camera = player;

        Emitter enemy_emitter;
        enemy_emitter.type = EmitterType::FOUNTAIN;
        enemy_emitter.min_start_angle = -180.0f / 180.0f * M_PI;
        enemy_emitter.max_start_angle = 180 / 180.0f * M_PI ;
        enemy_emitter.min_start_angular_velocity = 3;
        enemy_emitter.max_start_angular_velocity = 4;
        enemy_emitter.min_end_angular_velocity = 5;
        enemy_emitter.max_end_angular_velocity = 9;

        enemy_emitter.max_particles = 30;
        enemy_emitter.min_live_time = 0.7;
        enemy_emitter.max_live_time = 2.1;
        enemy_emitter.velocity_range = Ball{Vec3(0, 0, 0), 0.1};
        enemy_emitter.position_range = 0.05;
        enemy_emitter.start_color_range = Ball{Vec3(0.2, 0.6,1.0, 0.7), 0.0};
        enemy_emitter.end_color_range = Ball{Vec3(0.2, 0.6,1.0, 0.0), 0.0};
//        Particles enemy_particles;
        enemy = make_shared<Node>();
        enemy->name = "Enemy";
        enemy->flags = Node::Flags::NONE;
        enemy->phys = Node::PhysFlags::GHOST;
        enemy->particles_init(enemy_emitter, Texture().data("./res/snowflake.pvr"));
        scene.nodes.emplace_back(enemy);
        enemy->camera._pos = Vec3(0, 5, 3);


//        Emitter explosion_emitter;
//        explosion_emitter.type = EmitterType::EXPLOSION;
//        explosion_emitter.min_start_angle = -180.0f / M_PI;
//        explosion_emitter.max_start_angle = 180 / M_PI ;
//        explosion_emitter.min_start_angular_velocity = 3;
//        explosion_emitter.max_start_angular_velocity = 4;
//        explosion_emitter.min_end_angular_velocity = -5;
//        explosion_emitter.max_end_angular_velocity = 5;
//        explosion_emitter.position_range = 1.00;
//        explosion_emitter.min_start_size = 0;
//        explosion_emitter.max_start_size = 3;
//        explosion_emitter.min_end_size = 0.0;
//        explosion_emitter.max_end_size = 0.0;
//        explosion_emitter.max_particles = 600;
//        explosion_emitter.max_live_time = 0.7;
//        explosion_emitter.min_live_time = 0.9;
//        explosion_emitter.velocity_range = Ball{Vec3(0, 3, 0), 1};
//        explosion_emitter.gravity = Vec3(0, -2, 0);
//        explosion_emitter.start_color_range = Ball{Vec3(1, 1, 0.658, 1.0), 0.05};
//        explosion_emitter.end_color_range = Ball{Vec3(1,0,0, 1), 0.05};
        Emitter explosion_emitter;
        explosion_emitter.type = EmitterType::EXPLOSION;
        explosion_emitter.min_start_angle = -180.0f / M_PI;
        explosion_emitter.max_start_angle = 180 / M_PI ;
        explosion_emitter.min_start_angular_velocity = 3;
        explosion_emitter.max_start_angular_velocity = 4;
        explosion_emitter.min_end_angular_velocity = -5;
        explosion_emitter.max_end_angular_velocity = 5;
        explosion_emitter.position_range = 1.00;
        explosion_emitter.min_start_size = 0;
        explosion_emitter.max_start_size = 3;
        explosion_emitter.min_end_size = 1.0;
        explosion_emitter.max_end_size = 2.0;
        explosion_emitter.max_particles = 600;
        explosion_emitter.max_live_time = 0.7;
        explosion_emitter.min_live_time = 0.9;
        explosion_emitter.velocity_range = Ball{Vec3(0, 3, 0), 2};
        explosion_emitter.gravity = Vec3(0, -1, 0);
        explosion_emitter.start_color_range = Ball{Vec3(1, 1, 0.658, 1.0), 0.05};
        explosion_emitter.end_color_range = Ball{Vec3(1,0,0, 0), 0.05};
        explosion = make_shared<Node>();
        explosion->name = "Explosion";
        explosion->flags = Node::Flags::NONE;
        explosion->phys = Node::PhysFlags::GHOST;
        explosion->camera._pos = Vec3(-5, 3, 5);
        //    this->_tex.data("./res/smokeparticle.pvr");
        //    this->_tex.data("./res/fireparticle.pvr");
        //    this->_tex.data("./res/face.pvr");
        //    this->_tex.data("./res/old-lady.pvr");
//        explosion->particles.emitter = explosion_emitter;
        explosion->particles_init(explosion_emitter, Texture().data("./res/smokeparticle.pvr"));
//        explosion->particles_init(explosion_emitter, Texture().data("./res/fireparticle2.pvr"));

        // avoid updates for fully blown explosions (dead = num_particles)
        Emitter smoke_emitter;
        smoke_emitter.type = EmitterType::EXPLOSION;
        smoke_emitter.min_start_angle = -180.0f / 180.0f * M_PI;
        smoke_emitter.max_start_angle =  180.0f / 180.0f * M_PI ;
        smoke_emitter.min_start_angular_velocity = -20;
        smoke_emitter.max_start_angular_velocity =  20;
        smoke_emitter.min_end_angular_velocity = 0;
        smoke_emitter.max_end_angular_velocity = 0;
        smoke_emitter.position_range = 1.25;
        smoke_emitter.min_start_size = 0;
        smoke_emitter.max_start_size = 2;
        smoke_emitter.min_end_size = 1.0;
        smoke_emitter.max_end_size = 2.0;
//        smoke_emitter.min_end_size = 1.0;
//        smoke_emitter.max_end_size = 1.0;
        smoke_emitter.max_particles = 600;
        smoke_emitter.min_live_time = 1.7;
        smoke_emitter.max_live_time = 2.4;
        smoke_emitter.velocity_range = Ball{Vec3(0, 0, 0), 6};
        smoke_emitter.gravity = Vec3(0, 3.8, 0);
//        smoke_emitter.start_color_range = Ball{Vec3(1, 1, 0.658, 1.0), 0.05};
        smoke_emitter.start_color_range = Ball{Vec3(0.5, 0.5, 0.5, 0.1), 0.00};
        smoke_emitter.end_color_range = Ball{Vec3(0.0,0.0,0, 0), 0.00};
        smoke = make_shared<Node>();
        smoke->name = "Smoke";
        smoke->flags = Node::Flags::NONE;
        smoke->phys = Node::PhysFlags::GHOST;
        smoke->camera._pos = Vec3(-5, 3, 5);
        //    this->_tex.data("./res/smokeparticle.pvr");
        //    this->_tex.data("./res/fireparticle.pvr");
        //    this->_tex.data("./res/face.pvr");
        //    this->_tex.data("./res/old-lady.pvr");

//        explosion->particles.emitter = explosion_emitter;
//        explosion->particles_init(explosion_emitter, Texture().data("./res/smokeparticle.pvr"));
        smoke->particles_init(smoke_emitter, Texture().data("./res/smokeparticle.pvr"));


        Emitter fireball_emitter;
        fireball_emitter.type = EmitterType::FOUNTAIN;
        fireball_emitter.min_start_angle = -180.0f / M_PI;
        fireball_emitter.max_start_angle = 180 / M_PI ;
        fireball_emitter.min_start_angular_velocity = 3;
        fireball_emitter.max_start_angular_velocity = 4;
        fireball_emitter.min_end_angular_velocity = -5;
        fireball_emitter.max_end_angular_velocity = 5;
        fireball_emitter.position_range = 0.20 / 4.0f;
        fireball_emitter.min_start_size = 0 / 4.0f;
        fireball_emitter.max_start_size = 3 / 4.0f;
        fireball_emitter.min_end_size = 1.0 / 4.0f;
        fireball_emitter.max_end_size = 2.0 / 4.0f;
        fireball_emitter.max_particles = 500;
        fireball_emitter.max_live_time = 0.7;
        fireball_emitter.min_live_time = 0.9;
        fireball_emitter.velocity_range = Ball{Vec3(0, 3 / 4.0f, 0), 2 / 4.0f};
        fireball_emitter.gravity = Vec3(0, -1, 0);
        fireball_emitter.start_color_range = Ball{Vec3(1, 1, 0.658, 1.0), 0.05};
        fireball_emitter.end_color_range = Ball{Vec3(1,0,0, 0), 0.05};
        fireball = make_shared<Node>();
        fireball->name = "Fireball";
        fireball->flags = Node::Flags::NONE;
        fireball->phys = Node::PhysFlags::GHOST;
        fireball->camera._pos = Vec3(-3, 3, 3);
        //    this->_tex.data("./res/smokeparticle.pvr");
        //    this->_tex.data("./res/fireparticle.pvr");
        //    this->_tex.data("./res/face.pvr");
        //    this->_tex.data("./res/old-lady.pvr");
//        fireball->particles.emitter = fireball_emitter;
//        fireball->particles_init(fireball_emitter, Texture().data("./res/smokeparticle.pvr"));
        fireball->particles_init(fireball_emitter, Texture().data("./res/fireparticle2.pvr"));


        scene.nodes.emplace_back(explosion);
        scene.nodes.emplace_back(smoke);

        scene.nodes.emplace_back(fireball);

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
        glEnable(GL_BLEND);
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

//    uint vbo_random_points, vao_random_points;
//    uint current_point;

    float angleOY = 0.0;

    void loop(){
        bool quit = false;
        SDL_Event event;

        FPSCounter fps_counter;


        bool keys_pressed[SDL_NUM_SCANCODES];
        bool is_shift_pressed = false;
        bool is_ctrl_pressed = false;
        for (int i = 0; i < SDL_NUM_SCANCODES; i++) keys_pressed[i] = false;

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
                    level.fireball->camera.go(moving_speed);
                }
                if (keys_pressed[SDL_SCANCODE_DOWN]){
                    dir += Vec3(0, 0, moving_speed);
                    level.enemy->camera.go(-moving_speed);
                    level.fireball->camera.go(-moving_speed);
                }
                if (keys_pressed[SDL_SCANCODE_LEFT]){
                    dir += Vec3(-moving_speed, 0, 0);
                    level.enemy->camera.stride(-moving_speed);
                    level.fireball->camera.stride(-moving_speed);
                }
                if (keys_pressed[SDL_SCANCODE_RIGHT]){
                    dir += Vec3(moving_speed, 0, 0);
                    level.enemy->camera.stride(moving_speed);
                    level.fireball->camera.stride(moving_speed);
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
            SDL_Delay(12);

//            SDL_SetCursor()

            while( SDL_PollEvent( &event ) ){

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
                        level.explosion->particles.explode();
                        level.smoke->particles.explode();
                    }
                    is_shift_pressed = event.key.keysym.mod & KMOD_SHIFT;
                    is_ctrl_pressed = event.key.keysym.mod & KMOD_CTRL;

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

//#include "gapi/loaders/pvr.h"

//#define SDL_MAIN_HANDLED

//#include "math/matrix.h"
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
