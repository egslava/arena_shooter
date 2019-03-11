#ifndef LEVEL_H
#define LEVEL_H

#include "game/bullet.h"
#include "scene/scene.h"
#include "gapi/gapi.h"
#include "game/enemy.h"


struct Level {
    Scene scene;
    SPNode player, nebula;
    SPNodes enemies;
    Bullets bullets;
    Enemy enemy;

#ifndef NDEBUG
    SPNode axes;
#endif

    void init();
    void on_collision(SPNode &node1, SPNode &node2);
};


/**
 * @brief The AppCallback class
 * The reason of existing of this class - is just a decomposition. I believe, only
 * one instance of such class should exist, that's why it's not virtual (already virtual, actually),
 * but just binds directly.
 *
 * Feel free to make its method virtuals and use inheritance in case of, for instance,
 * several levels and so on.
 */

struct MyAppCallback : public AppCallback{
    Level level;

    void on_after_init();
    void on_mousemove(double dx, double dy);
    void on_mousewheel(double d);
    void on_mousedown();
    void on_keydown(SDL_Scancode scancode);
    void on_tick(double tick_time);
    ~MyAppCallback(){}
};


#endif // LEVEL_H
