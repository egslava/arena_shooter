#ifndef BULLET_H
#define BULLET_H

#include "scene/scene.h"
#include "scene/camera.h"
#include "math/math.h"
//#include "game/level.h"


class Bullet {
    SPNode _node_particles;
    SPNode _node_smoke;
    double _start_time;
    double _last_update_time;
    Vec3 _dir;
    Vec3 _pos;

    bool _is_exploded = false;

    Emitter fireball_emitter;
    Emitter explosion_emitter;
    Emitter smoke_emitter;
    bool _should_explode();
public:
    void init(Scene &scene);
    void fire(const Camera &from);
    void _explode();
    void update();
    bool is_dead()const;
};


class Bullets{
    Scene *_scene;
    std::vector<Bullet> _bullets;
public:
    void init(Scene &scene) {
        this->_scene = &scene;
        this->_bullets.reserve(20);
        for (int i = 0; i < 20; i++){
            Bullet new_one;
            new_one.init(*(this->_scene));
            this->_bullets.push_back(new_one);
        }
    };
    void fire(const Camera &from);
    void update();
};



class Hero{};

class Game {
    Bullets bullets;
    Hero  player;
//    Level level;

    void onStart(){
//        level.init();
    }

    void onUpdate(double dt){
        bullets.update();
    }

    void onInput(){

    }

    void onCollision(SPNode node1, SPNode node2){

    }
};

#endif // BULLET_H
