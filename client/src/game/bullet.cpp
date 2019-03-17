#include "game/game.h"

//class



void Bullet::init(Scene &scene)
{
//    this->fireball_emitter.type = EmitterType::FOUNTAIN;
//    this->fireball_emitter.min_start_angle = -180.0f / M_PI;
//    this->fireball_emitter.max_start_angle = 180 / M_PI ;
//    this->fireball_emitter.min_start_angular_velocity = 3;
//    this->fireball_emitter.max_start_angular_velocity = 4;
//    this->fireball_emitter.min_end_angular_velocity = -5;
//    this->fireball_emitter.max_end_angular_velocity = 5;
//    this->fireball_emitter.position_range = 0.20 / 4.0f;
//    this->fireball_emitter.min_start_size = 0 / 4.0f;
//    this->fireball_emitter.max_start_size = 3 / 4.0f;
//    this->fireball_emitter.min_end_size = 1.0 / 4.0f;
//    this->fireball_emitter.max_end_size = 2.0 / 4.0f;
//    this->fireball_emitter.max_particles = 500;
//    this->fireball_emitter.max_live_time = 0.7;
//    this->fireball_emitter.min_live_time = 0.9;
//    this->fireball_emitter.velocity_range = Ball{Vec3(0, 3 / 4.0f, 0), 2 / 4.0f};
//    this->fireball_emitter.gravity = Vec3(0, -1, 0);
//    this->fireball_emitter.start_color_range = Ball{Vec3(1, 1, 0.658, 1.0), 0.05};
//    this->fireball_emitter.end_color_range = Ball{Vec3(1,0,0, 0), 0.05};

    this->fireball_emitter.type = EmitterType::FOUNTAIN;
    this->fireball_emitter.min_start_angle = -180.0f / M_PI;
    this->fireball_emitter.max_start_angle = 180 / M_PI ;
    this->fireball_emitter.min_start_angular_velocity = 3;
    this->fireball_emitter.max_start_angular_velocity = 4;
    this->fireball_emitter.min_end_angular_velocity = -5;
    this->fireball_emitter.max_end_angular_velocity = 5;
    this->fireball_emitter.position_range = 0.20 / 4.0f;
    this->fireball_emitter.min_start_size = 0 / 4.0f;
    this->fireball_emitter.max_start_size = 3 / 4.0f;
    this->fireball_emitter.min_end_size = 1.0 / 4.0f;
    this->fireball_emitter.max_end_size = 2.0 / 4.0f;
    this->fireball_emitter.max_particles = 500;
    this->fireball_emitter.max_live_time = 0.7;
    this->fireball_emitter.min_live_time = 0.9;
    this->fireball_emitter.velocity_range = Ball{Vec3(0, 3 / 4.0f, 0), 2 / 4.0f};
    this->fireball_emitter.gravity = Vec3(0, -1, 0);
    this->fireball_emitter.start_color_range = Ball{Vec3(1, 1, 0.658, 1.0), 0.05};
    this->fireball_emitter.end_color_range = Ball{Vec3(1,0,0, 0), 0.05};

    this->explosion_emitter.type = EmitterType::EXPLOSION;
    this->explosion_emitter.min_start_angle = -180.0f / M_PI;
    this->explosion_emitter.max_start_angle = 180 / M_PI ;
    this->explosion_emitter.min_start_angular_velocity = 3;
    this->explosion_emitter.max_start_angular_velocity = 4;
    this->explosion_emitter.min_end_angular_velocity = -5;
    this->explosion_emitter.max_end_angular_velocity = 5;
    this->explosion_emitter.position_range = 1.00;
    this->explosion_emitter.min_start_size = 0;
    this->explosion_emitter.max_start_size = 3;
    this->explosion_emitter.min_end_size = 1.0;
    this->explosion_emitter.max_end_size = 2.0;
    this->explosion_emitter.max_particles = 600;
    this->explosion_emitter.max_live_time = 0.7;
    this->explosion_emitter.min_live_time = 0.9;
    this->explosion_emitter.velocity_range = Ball{Vec3(0, 3, 0), 2};
    this->explosion_emitter.gravity = Vec3(0, -1, 0);
    this->explosion_emitter.start_color_range = Ball{Vec3(1, 1, 0.658, 1.0), 0.05};
    this->explosion_emitter.end_color_range = Ball{Vec3(1,0,0, 0), 0.05};



//    this->smoke_emitter.type = EmitterType::EXPLOSION;
//    this->smoke_emitter.min_start_angle = -180.0f / 180.0f * M_PI;
//    this->smoke_emitter.max_start_angle =  180.0f / 180.0f * M_PI ;
//    this->smoke_emitter.min_start_angular_velocity = -20;
//    this->smoke_emitter.max_start_angular_velocity =  20;
//    this->smoke_emitter.min_end_angular_velocity = 0;
//    this->smoke_emitter.max_end_angular_velocity = 0;
//    this->smoke_emitter.position_range = 1.25;
//    this->smoke_emitter.min_start_size = 0;
//    this->smoke_emitter.max_start_size = 2;
//    this->smoke_emitter.min_end_size = 1.0;
//    this->smoke_emitter.max_end_size = 2.0;
////        smoke_emitter.min_end_size = 1.0;
////        smoke_emitter.max_end_size = 1.0;
//    this->smoke_emitter.max_particles = 600;
//    this->smoke_emitter.min_live_time = 1.7;
//    this->smoke_emitter.max_live_time = 2.4;
//    this->smoke_emitter.velocity_range = Ball{Vec3(0, 0, 0), 6};
//    this->smoke_emitter.gravity = Vec3(0, 3.8, 0);
////        smoke_emitter.start_color_range = Ball{Vec3(1, 1, 0.658, 1.0), 0.05};
//    this->smoke_emitter.start_color_range = Ball{Vec3(0.5, 0.5, 0.5, 0.1), 0.00};
//    this->smoke_emitter.end_color_range = Ball{Vec3(0.0,0.0,0, 0), 0.00};




    this->smoke_emitter.type = EmitterType::EXPLOSION;
    this->smoke_emitter.min_start_angle = -180.0f / 180.0f * M_PI;
    this->smoke_emitter.max_start_angle =  180.0f / 180.0f * M_PI ;
    this->smoke_emitter.min_start_angular_velocity = -20;
    this->smoke_emitter.max_start_angular_velocity =  20;
    this->smoke_emitter.min_end_angular_velocity = 0;
    this->smoke_emitter.max_end_angular_velocity = 0;
    this->smoke_emitter.position_range = 1.25;
    this->smoke_emitter.min_start_size = 0;
    this->smoke_emitter.max_start_size = 2;
    this->smoke_emitter.min_end_size = 1.0;
    this->smoke_emitter.max_end_size = 2.0;
//        smoke_emitter.min_end_size = 1.0;
//        smoke_emitter.max_end_size = 1.0;
    this->smoke_emitter.max_particles = 600;
    this->smoke_emitter.min_live_time = 1.7;
    this->smoke_emitter.max_live_time = 2.4;
    this->smoke_emitter.velocity_range = Ball{Vec3(0, 0, 0), 6};
    this->smoke_emitter.gravity = Vec3(0, 3.8, 0);
//        smoke_emitter.start_color_range = Ball{Vec3(1, 1, 0.658, 1.0), 0.05};
    this->smoke_emitter.start_color_range = Ball{Vec3(0.5, 0.5, 0.5, 0.1), 0.00};
    this->smoke_emitter.end_color_range = Ball{Vec3(0.0,0.0,0, 0), 0.00};


    SPNode explosion = make_shared<Node>();
    explosion->name = "Explosion";
    explosion->flags = Node::Flags::NONE;
    explosion->phys = Node::PhysFlags::GHOST;
    explosion->camera._pos = Vec3(-5, 3, 5);

    this->_node_fireball = make_shared<Node>();
    this->_node_fireball->name = "Fireball";
    this->_node_fireball->flags = Node::Flags::NONE;
    this->_node_fireball->phys = Node::PhysFlags::COLLIDE_DYNAMIC;
    this->_node_fireball->camera._pos = Vec3(-3, 3, 3);
    this->_node_fireball->radius = 0.3;

//    this->_node_particles->
    //    this->_tex.data("./res/smokeparticle.pvr");
    //    this->_tex.data("./res/fireparticle.pvr");
    //    this->_tex.data("./res/face.pvr");
    //    this->_tex.data("./res/old-lady.pvr");
    //        fireball->particles.emitter = fireball_emitter;
    //        fireball->particles_init(fireball_emitter, Texture().data("./res/smokeparticle.pvr"));
    this->_node_fireball->particles_init(fireball_emitter, Texture().data("./res/fireparticle2.pvr"));

    this->_node_fireball->visible = false;
    scene.nodes.emplace_back(this->_node_fireball);

    this->_node_smoke = make_shared<Node>();
    this->_node_smoke->name = "Smoke";
    this->_node_smoke->flags = Node::Flags::NONE;
//    this->_node_smoke->phys = Node::PhysFlags::COLLIDE_DYNAMIC;
    this->_node_smoke->camera._pos = Vec3(-5, 3, 5);
    this->_node_smoke->visible = false;
    this->_node_smoke->particles_init(smoke_emitter, Texture().data("./res/smokeparticle.pvr"));
    scene.nodes.emplace_back(this->_node_smoke);

    this->_node_smoke->uda_group = UDA_BULLET;
//    this->_node_smoke->phys = Node::PhysFlags::COLLIDE_DYNAMIC;
    this->_node_fireball->uda_group = UDA_BULLET;
}

double _now(){
    return     ((double)(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count())) / 1e9;
}
void Bullet::fire(const Camera &from, int uda_who)
{
    this->_pos = from._pos;
    this->_dir = Vec3(from.getMatCameraToWorld() * Vec3(0, 0, -1)) - this->_pos;

    this->_pos += this->_dir*1.5;  // *2 so, the bullet doesn't kill the enemy immediately, when the enemy make a shot

    this->_start_time = _now();
    this->_last_update_time = this->_start_time;
    this->_is_exploded = false;
    this->_node_fireball->camera._pos = this->_pos;

    this->_node_smoke->visible = false;
    this->_node_fireball->visible = true;

    this->_node_fireball->particles.emitter = this->fireball_emitter;    // stop
    this->_node_smoke->particles.reinit(from._pos);

    this->_node_fireball->particles.emitter.type = EmitterType::FOUNTAIN;    // start
    this->_node_fireball->particles.reinit(this->_pos);

    this->_uda_who = uda_who;
}

void Bullet::_explode()
{
    this->_node_fireball->particles.emitter = this->explosion_emitter;    // stop
//    this->_node_particles->particles.reinit(this->_pos);
//    this->_node_particles->particles.explode();
    this->_node_smoke->visible = true;
    this->_node_smoke->camera._pos = this->_pos;
    this->_node_smoke->particles.reinit(this->_pos);
    this->_node_smoke->particles.explode();
    this->_is_exploded = true;
}

bool Bullet::_should_explode()
{
    return _now() - this->_start_time > 1.5;
}

bool Bullet::is_dead() const
{
    return _now() - this->_start_time > 8;  // time enough to go up to the skies and, then, crush on the ground
}

void Bullet::update()
{
    if (this->is_dead()){
        this->_node_fireball->visible = false;
        this->_node_smoke->visible = false;
        return;
    }

//    if (this->_should_explode()){
//        if (this->_is_exploded) return;

//        this->_explode();
//        return;
//    }

    double dt = _now() - this->_last_update_time;
    this->_last_update_time = _now();
    this->_dir += Vec3(0, -0.3, 0)*dt;
    this->_pos += 10 * dt * this->_dir;

    this->_node_fireball->camera._pos = this->_pos;

}


int Bullets::find(SPNode &by)
{
    for (int i = 0; i < this->_bullets.size(); i++) {
        const Bullet &bullet = this->_bullets[i];

        if (bullet._node_fireball == by || bullet._node_smoke == by){
            return i;
        }
    }
    return -1;

}

void Bullets::fire(const Camera &from, int uda_who)
{
    /* Calls Bullet::fire ;
     * @brief idx
     */
    int idx = -1;
    for (int i = 0; i < this->_bullets.size(); i++){
        const Bullet &bullet = this->_bullets[i];
        if (bullet.is_dead()){
            idx = i;
            break;
        }
    }

    if (idx == -1){
        printf("Creating a new bullet\n");
        fflush(stdout);
        Bullet new_one;
        new_one.init(*(this->_scene));
        this->_bullets.push_back(new_one);
        idx = this->_bullets.size() - 1;
    }

//    printf("Before fire\n");
//    fflush(stdout);
    this->_bullets[idx].fire(from, uda_who);

//    printf("After fire\n");
//    fflush(stdout);
}

void Bullets::update()
{
    for(Bullet &bullet : this->_bullets){
        bullet.update();
    }
}

#include "game/level.h"

void Bullets::on_collision(SPNode &node1, SPNode &node2)
{
    bool _1_bullet = node1->uda_group == UDA_BULLET;
    bool _2_bullet = node2->uda_group == UDA_BULLET;

    // case 1: no bullets
    if (!_1_bullet && !_2_bullet) return;

    // case 2: only one of them is bullet
    if (!_1_bullet || !_2_bullet) {
        int idx = -1;
        SPNode &another_node = _1_bullet ? node1 : node2;
        idx = this->find(_1_bullet ? node1 : node2);

//         if (node2->phys == Node::PhysFlags::GHOST) return;

        if (!this->_bullets[idx]._is_exploded){
            this->_bullets[idx]._explode();
        }


    }

//    // case 3: both bullets
//    Bullet *bullet_node1 = dynamic_cast<Bullet*>(node1);
//    Bullet *bullet_node2 = dynamic_cast<Bullet*>(node2);

//    if (bullet_node1 != null){
//        bullet_node1->
//    }
}
