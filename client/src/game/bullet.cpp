#include "game/bullet.h"

//class



void Bullet::init(Scene &scene)
{
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

//    SPNode explosion = make_shared<Node>();
//    explosion->name = "Explosion";
//    explosion->flags = Node::Flags::NONE;
//    explosion->phys = Node::PhysFlags::GHOST;
//    explosion->camera._pos = Vec3(-5, 3, 5);

    this->_node_particles = make_shared<Node>();
    this->_node_particles->name = "Fireball";
    this->_node_particles->flags = Node::Flags::NONE;
    this->_node_particles->phys = Node::PhysFlags::GHOST;
    this->_node_particles->camera._pos = Vec3(-3, 3, 3);
    //    this->_tex.data("./res/smokeparticle.pvr");
    //    this->_tex.data("./res/fireparticle.pvr");
    //    this->_tex.data("./res/face.pvr");
    //    this->_tex.data("./res/old-lady.pvr");
    //        fireball->particles.emitter = fireball_emitter;
    //        fireball->particles_init(fireball_emitter, Texture().data("./res/smokeparticle.pvr"));
    this->_node_particles->particles_init(fireball_emitter, Texture().data("./res/fireparticle2.pvr"));

    this->_node_particles->visible = false;
    scene.nodes.emplace_back(this->_node_particles);


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
    this->_node_smoke = make_shared<Node>();
    this->_node_smoke->name = "Smoke";
    this->_node_smoke->flags = Node::Flags::NONE;
    this->_node_smoke->phys = Node::PhysFlags::GHOST;
    this->_node_smoke->camera._pos = Vec3(-5, 3, 5);
    this->_node_smoke->visible = false;
    this->_node_smoke->particles_init(smoke_emitter, Texture().data("./res/smokeparticle.pvr"));
    scene.nodes.emplace_back(this->_node_smoke);
}

double _now(){
    return     ((double)(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count())) / 1e9;
}
void Bullet::fire(const Camera &from)
{

    this->_is_exploded = false;
    this->_node_particles->particles.emitter = this->fireball_emitter;    // stop

//    this->_node_particles->particles.emitter.type = EmitterType::EXPLOSION;    // start
    this->_node_particles->particles.emitter.type = EmitterType::FOUNTAIN;    // start
    this->_node_particles->particles.init(this->_pos);

    this->_start_time = _now();
    this->_last_update_time = this->_start_time;

    this->_node_particles->visible = true;

    this->_pos = Vec3(from._pos);
    this->_dir = Vec3(from.getMatCameraToWorld() * Vec3(0, 0, -1)) - this->_pos;

    this->_node_particles->camera._pos = this->_pos;
}

bool Bullet::_should_explode()
{
    return _now() - this->_start_time > 2;
}

bool Bullet::is_dead() const
{
return _now() - this->_start_time > 4;
}

void Bullet::update()
{
    if (this->is_dead()){
        this->_node_particles->visible = false;
        this->_node_smoke->visible = false;
        return;
    }

    if (this->_should_explode()){
        if (this->_is_exploded) return;

        this->_node_particles->particles.emitter = this->explosion_emitter;    // stop
        this->_node_particles->particles.explode();
        this->_node_smoke->visible = true;
        this->_node_smoke->camera._pos = this->_pos;
        this->_node_smoke->particles.explode();
        this->_is_exploded = true;
        return;
    }

    double dt = _now() - this->_last_update_time;
    this->_last_update_time = _now();
    this->_dir += Vec3(0, -0.3, 0)*dt;
    this->_pos += 10 * dt * this->_dir;

    this->_node_particles->camera._pos = this->_pos;

}


void Bullets::fire(const Camera &from)
{
    int idx = -1;
    for (int i = 0; i < this->_bullets.size(); i++){
        const Bullet &bullet = this->_bullets[i];
        if (bullet.is_dead()){
            idx = i;
            break;
        }
    }

    if (idx == -1){
        Bullet new_one;
        new_one.init(*(this->_scene));
        this->_bullets.push_back(new_one);
        idx = this->_bullets.size() - 1;
    }

    this->_bullets[idx].fire(from);
}

void Bullets::update()
{
    for(Bullet &bullet : this->_bullets){
        bullet.update();
    }
}
