#ifndef WININIT_H
#define WININIT_H

#include <SDL2/SDL.h>
#include "exceptions.h"

class MySDLApp;
struct AppCallback;


#define SDL_SAFE(code) do { \
    int result = code;\
    if (result == -1){ \
        throw MySDLException(#code);\
    } \
}while(false);


struct AppCallback{
    int screen_width, screen_height;
    bool keys_pressed[SDL_NUM_SCANCODES];
    bool is_shift_pressed = false;
    bool is_ctrl_pressed = false;

    bool should_shut_down = false;  // duct tape. Since I can't call MySDLApp methods here or SDL_Quit();

    virtual void on_mousemove(double dx, double dy) = 0;
    virtual void on_mousewheel(double d) = 0;
    virtual void on_after_init() = 0;
    virtual void on_mousedown() = 0;
    virtual void on_keydown(SDL_Scancode scancode) = 0;
    virtual void on_tick(double tick_time) = 0;
    virtual ~AppCallback(){}
};


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

    AppCallback &callback;
public:

    MySDLApp(AppCallback &callback);
    ~MySDLApp();

    void loop();
};

#endif // WININIT_H
