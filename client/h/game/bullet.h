#ifndef BULLET_H
#define BULLET_H

#include "scene/scene.h"
#include "scene/camera.h"
#include "math/math.h"

struct Bullets;


class Bullet;
class Bullets;

class Bullet {
    SPNode _node_fireball;
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
    friend class Bullets;
public:
    void init(Scene &scene);
    void fire(const Camera &from);
    void _explode();
    void update();
    bool is_dead()const;
};


class Bullets{
    Scene *_scene;
public:
    std::vector<Bullet> _bullets;
    int find(SPNode &by);
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
    void on_collision(SPNode &node1, SPNode &node2);
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


#include "game/level.h"

#endif // BULLET_H
