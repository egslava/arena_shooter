#ifndef ENEMY_H
#define ENEMY_H

#include "gapi/gapi.h"

class Enemy {
public:
    bool is_disabled = true;
    std::deque<Vec3> _last_pos;
    SPNode _enemy;
    void init(Scene &scene);
    void on_tick(double tick_time){
    }
};


#endif // ENEMY_H
