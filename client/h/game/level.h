#ifndef LEVEL_H
#define LEVEL_H

#include "game/bullet.h"
#include "scene/scene.h"
#include "gapi/gapi.h"
#include "game/enemy.h"


struct Level;
struct LevelState;
struct MyAppCallback;

struct _GameOverLevelState;
struct _GamePlayLevelState;


struct LevelState {
    Level &level;
    LevelState(Level &level): level(level){}
    virtual ~LevelState(){}

    virtual void on_enter() = 0;
    virtual void on_exit() = 0;
    virtual void on_tick(double tick_time) = 0;
};

struct _GameOverLevelState : public LevelState {
    _GameOverLevelState(Level &level);
    virtual ~_GameOverLevelState(){}

    virtual void on_tick(double tick_time);
    virtual void on_enter();
    virtual void on_exit();
};


struct _GamePlayLevelState : public LevelState {
    _GamePlayLevelState(Level &level);
    virtual void on_enter();
    virtual void on_exit();
    virtual void on_tick(double tick_time);
};


struct Level {
    Scene scene;
    SPNode player, nebula;
    std::vector<Enemy> enemies;
    Bullets bullets;
    Enemy enemy;

    SPNode gameover, gameover_black_overlap;

#ifndef NDEBUG
    SPNode axes;
#endif

    Level();
    void init(MyAppCallback *cb, int screen_width, int screen_height);
    void on_collision(SPNode &node1, SPNode &node2);

    LevelState *_state_current = &_state_gameplay;
private:
    friend class _GameOverLevelState;
    friend class _GamePlayLevelState;
    MyAppCallback *_callback;

    // states
    _GameOverLevelState _state_gameover;
    _GamePlayLevelState _state_gameplay;
    void _change_state(LevelState &state) {
        _state_current->on_exit();
        _state_current = &state;
        _state_current->on_enter();
    }

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
