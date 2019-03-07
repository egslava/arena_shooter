#include "gapi/wininit.h"

#include "stdafx.h"
#include "fpscounter.h"

MySDLApp::MySDLApp(AppCallback &callback) : callback(callback) {

    for (int i = 0; i < SDL_NUM_SCANCODES; i++) {
        callback.keys_pressed[i] = false;
    }

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
    SDL_SAFE(SDL_GL_SetSwapInterval(0));  // vsync
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

    callback.on_after_init();
}

void MySDLApp::loop(){
    SDL_Event event;

    FPSCounter fps_counter;
    float s=1.0;

    using timer=std::chrono::high_resolution_clock ;
    timer::time_point frame_start = timer::now();
    double tick_time = 0;
    while(!callback.should_shut_down){
        tick_time = static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(timer::now() - frame_start).count()) / 1000000000.0 ;
        frame_start = timer::now();

        fps_counter.begin();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        callback.on_tick(tick_time);

        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CCW);
        glCullFace(GL_BACK);

        fps_counter.end();
        SDL_GL_SwapWindow(_window);
        //            SDL_Delay(12);

        while( SDL_PollEvent( &event ) ){

            if (event.type == SDL_MOUSEBUTTONDOWN){
                callback.on_mousedown();
            }
            if (event.type == SDL_KEYDOWN){
                callback.keys_pressed[event.key.keysym.scancode] = true;
                callback.is_shift_pressed = event.key.keysym.mod & KMOD_SHIFT;
                callback.is_ctrl_pressed = event.key.keysym.mod & KMOD_CTRL;

                if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE){
                    callback.should_shut_down = true;
                }

                callback.on_keydown(event.key.keysym.scancode);
            }

            if (event.type == SDL_KEYUP){
                callback.keys_pressed[event.key.keysym.scancode] = false;
            }

            if (event.type == SDL_MOUSEMOTION){
                Sint32 x = event.motion.x, y = event.motion.y;

                float deltaOY = static_cast<float>(event.motion.xrel) * 0.01;
                float deltaOX = static_cast<float>(event.motion.yrel) * 0.01;

                callback.on_mousemove(deltaOX, deltaOY);

                if (x < viewport_width * 0.1 || x > viewport_width * 0.9 || y < viewport_height * 0.1 || y > viewport_height * 0.9 ){
                    SDL_WarpMouseInWindow(_window, viewport_width / 2, viewport_height / 2);
                    while(SDL_PollEvent( &event ));

                }
            }

            if (event.type == SDL_MOUSEWHEEL){
                callback.on_mousewheel(event.wheel.y);
            }

            if( event.type == SDL_QUIT ){
                callback.should_shut_down = true;
            }
        }
    }
    //        SDL_GL_SwapWindow(_window);
}

MySDLApp::~MySDLApp(){
    SDL_GL_DeleteContext(_gl_context);
    SDL_DestroyWindow(_window);
    SDL_Quit();
}
