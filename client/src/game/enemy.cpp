#include "game/game.h"

void Enemy::init(Scene &scene){
    Emitter enemy_emitter;
    enemy_emitter.type = EmitterType::FOUNTAIN;
    enemy_emitter.min_start_angle = -180.0f / 180.0f * M_PI;
    enemy_emitter.max_start_angle = 180 / 180.0f * M_PI ;
    enemy_emitter.min_start_angular_velocity = -3;
    enemy_emitter.max_start_angular_velocity = 4;
    enemy_emitter.min_end_angular_velocity = 5;
    enemy_emitter.max_end_angular_velocity = 9;

    enemy_emitter.max_particles = 30;
    enemy_emitter.min_live_time = 0.3;
    enemy_emitter.max_live_time = 0.7;
//    enemy_emitter.max_live_time = 2.1;
    enemy_emitter.velocity_range = Ball{Vec3(0, 0, 0), 0.1};
    enemy_emitter.position_range = 0.05;
    enemy_emitter.start_color_range = Ball{Vec3(0.2, 0.6,1.0, 0.9), 0.0};
    enemy_emitter.end_color_range = Ball{Vec3(0.2, 0.6,1.0, 0.0), 0.0};
    //        Particles enemy_particles;
    _enemy = make_shared<Node>();
    _enemy->model.load("./res/enemy/enemy.model", Texture().data("./res/enemy/diffuse.pvr"));
//    _enemy->model.load("./res/debug/axes.model", Texture().data("./res/debug/axes.pvr"));
    _enemy->camera._pos = Vec3(0, 5, 0);
    _enemy->radius = 0.9;
//    _enemy->radius = 1.85;
    _enemy->name = "Enemy";
    _enemy->flags = Node::Flags::NONE;
    _enemy->uda_group = UDA_ENEMY;
    _enemy->phys = Node::PhysFlags::COLLIDE_DYNAMIC | Node::PhysFlags::PULL_AWAY | Node::PhysFlags::GRAVITY | Node::PhysFlags::AABB_ONLY | Node::PhysFlags::AABB_BY_RAD;
    _enemy->particles_init(enemy_emitter, Texture().data("./res/snowflake.pvr"));

    scene.nodes.emplace_back(_enemy);
}
