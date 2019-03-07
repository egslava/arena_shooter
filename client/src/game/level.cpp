#include "game/level.h"

void print_collides(SPNode node1, SPNode node2){
//    printf("Collision: %s and %s\n", node1->name, node2->name);
}

void Level::init(){
    scene.init();
    scene.on_collision = [this](SPNode node1, SPNode node2) { this->on_collision(node1, node2); };

    scene.nodes.emplace_back(new Node{"Ground", Node::Flags::NONE, Node::PhysFlags::COLLIDE_STATIC, (Model().load("res/level/Ground.model", Texture().data("./res/level/textures/Ground Light Tex2.pvr")/*, Color(WHITE)*/))});
    //        scene.nodes.emplace_back(new Nododel().load("res/leNode::Flags::RIGID, e{(Mvel1/Ground.model", Texture().data("./res/level/textures/Ground.pvr")/*, Color(WHITE)*/))});

    nebula = make_shared<Node>();
    nebula->name = "Nebula";
    nebula->phys = Node::PhysFlags::GHOST;
    nebula->flags = Node::Flags::NONE;
    nebula->model = std::move(Model().load("res/level/Nebula_Sky.model", Texture().data("./res/level/textures/nebula.pvr")));
    scene.nodes.emplace_back(nebula);


    scene.nodes.emplace_back(new Node{"Flower", Node::Flags::NONE, Node::PhysFlags::COLLIDE_STATIC, (Model().load("res/level/Flower.model", Texture().data("./res/level/textures/flower_lm.pvr")/*, Color(CYAN)*/).color(Vec3(0.063, 0.041, 0.402).bright_rgb(1)))});
    //        scene.nodes.emplace_back(new Nododel().load("res/leNode::Flags::RIGID, e{(Mvel1/stairs.model", Texture().data("./res/level/textures/stairs.pvr")/*, Color(WHITE)*/))});
    scene.nodes.emplace_back(new Node{"Roof", Node::Flags::NONE, Node::PhysFlags::COLLIDE_STATIC, (Model().load("res/level/Roof.model", Texture().data("./res/level/textures/TableLightMap.pvr")/*, Color(RED/PINK)*/).color(Vec3(1, 1, 1).bright_rgb(1)))});
    scene.nodes.emplace_back(new Node{"Roof top", Node::Flags::NONE, Node::PhysFlags::COLLIDE_STATIC, (Model().load("res/level/RoofTop.model", Texture().data("./res/level/textures/RoofLightMap.pvr")/*, Color(RED/PINK)*/).color(Vec3(0.319, 0, 0.003).bright_rgb(3)))});
    scene.nodes.emplace_back(new Node{"Crystal", Node::Flags::NONE, Node::PhysFlags::COLLIDE_STATIC, (Model().load("res/level/Crystal_Big.model", Texture().data("./res/level/textures/color_white.pvr")/*, Color(RED/PINK)*/))});
    scene.nodes.emplace_back(new Node{"Crystal", Node::Flags::NONE, Node::PhysFlags::COLLIDE_STATIC, (Model().load("res/level/Crystal_Top.model", Texture().data("./res/level/textures/color_white.pvr")/*, Color(RED/PINK)*/))});
    scene.nodes.emplace_back(new Node{"Crystal", Node::Flags::NONE, Node::PhysFlags::COLLIDE_STATIC, (Model().load("res/level/CrystalBottom.model", Texture().data("./res/level/textures/color_white.pvr")/*, Color(RED/PINK)*/))});
    scene.nodes.emplace_back(new Node{"BallLight1", Node::Flags::NONE, Node::PhysFlags::COLLIDE_STATIC, (Model().load("res/level/BallLight1.model", Texture().data("./res/level/textures/color_white.pvr")/*, Color(RED/PINK)*/))});
    scene.nodes.emplace_back(new Node{"BallLight2", Node::Flags::NONE, Node::PhysFlags::COLLIDE_STATIC, (Model().load("res/level/BallLight2.model", Texture().data("./res/level/textures/color_white.pvr")/*, Color(RED/PINK)*/))});
    scene.nodes.emplace_back(new Node{"BallLight3", Node::Flags::NONE, Node::PhysFlags::COLLIDE_STATIC, (Model().load("res/level/BallLight3.model", Texture().data("./res/level/textures/color_white.pvr")/*, Color(RED/PINK)*/))});
    scene.nodes.emplace_back(new Node{"BallLight4", Node::Flags::NONE, Node::PhysFlags::COLLIDE_STATIC, (Model().load("res/level/BallLight4.model", Texture().data("./res/level/textures/color_white.pvr")/*, Color(RED/PINK)*/))});

    scene.nodes.emplace_back(new Node{"Stairs", Node::Flags::NONE, Node::PhysFlags::COLLIDE_STATIC, (Model().load("res/level/Stairs1.model", Texture().data("./res/level/textures/stairs_lightmap1.pvr")/*, Color(RED/PINK)*/))});
    scene.nodes.emplace_back(new Node{"Stairs", Node::Flags::NONE, Node::PhysFlags::COLLIDE_STATIC, (Model().load("res/level/Stairs2.model", Texture().data("./res/level/textures/stairs_lightmap2.pvr")/*, Color(RED/PINK)*/))});
    scene.nodes.emplace_back(new Node{"Stairs", Node::Flags::NONE, Node::PhysFlags::COLLIDE_STATIC, (Model().load("res/level/Stairs3.model", Texture().data("./res/level/textures/stairs_lightmap3.pvr")/*, Color(RED/PINK)*/))});
    scene.nodes.emplace_back(new Node{"Stairs", Node::Flags::NONE, Node::PhysFlags::COLLIDE_STATIC, (Model().load("res/level/Stairs4.model", Texture().data("./res/level/textures/stairs_lightmap4.pvr")/*, Color(RED/PINK)*/))});
    scene.nodes.emplace_back(new Node{"Sphere", Node::Flags::NONE, Node::PhysFlags::COLLIDE_STATIC, (Model().load("res/debug/sphere_r1.model", Texture().data("./res/debug/grid.pvr")/*, Color(RED/PINK)*/))});

    player = make_shared<Node>();
    player->name = "Player";
    player->flags = Node::Flags::NONE;
    player->phys = Node::PhysFlags::COLLIDE_DYNAMIC | Node::PhysFlags::GRAVITY;
    player->radius = 0.90 * 1.0f;
    //        player->camera._pos = Vec3(0, 2, 0);

    scene.nodes.emplace_back(player);
    scene._camera = player;

    Emitter enemy_emitter;
    enemy_emitter.type = EmitterType::FOUNTAIN;
    enemy_emitter.min_start_angle = -180.0f / 180.0f * M_PI;
    enemy_emitter.max_start_angle = 180 / 180.0f * M_PI ;
    enemy_emitter.min_start_angular_velocity = 3;
    enemy_emitter.max_start_angular_velocity = 4;
    enemy_emitter.min_end_angular_velocity = 5;
    enemy_emitter.max_end_angular_velocity = 9;

    enemy_emitter.max_particles = 30;
    enemy_emitter.min_live_time = 0.7;
    enemy_emitter.max_live_time = 2.1;
    enemy_emitter.velocity_range = Ball{Vec3(0, 0, 0), 0.1};
    enemy_emitter.position_range = 0.05;
    enemy_emitter.start_color_range = Ball{Vec3(0.2, 0.6,1.0, 0.7), 0.0};
    enemy_emitter.end_color_range = Ball{Vec3(0.2, 0.6,1.0, 0.0), 0.0};
    //        Particles enemy_particles;
    enemy = make_shared<Node>();
    enemy->name = "Enemy";
    enemy->flags = Node::Flags::NONE;
    enemy->phys = Node::PhysFlags::GHOST;
    enemy->particles_init(enemy_emitter, Texture().data("./res/snowflake.pvr"));
    scene.nodes.emplace_back(enemy);
    enemy->camera._pos = Vec3(0, 5, 0);
    bullets.init(scene);

}

void Level::on_collision(SPNode &node1, SPNode &node2)
{
    bullets.on_collision(node1, node2);
//    printf("Collision: %s and %s\n", node1->name, node2->name);
}

void MyAppCallback::on_mousemove(double dx, double dy){
    level.player->camera.turn_left(-dy);
    level.player->camera.turn_up(-dx);
}

void MyAppCallback::on_mousewheel(double d){
    //        s += d / 10.f;
}

void MyAppCallback::on_after_init(){
    level.init();
    level.player->camera.turn_up(0.5 * M_PI);
    level.player->camera.fly(1.001);  // TODO: remove this ducktape
    level.player->camera.turn_up(-1. * M_PI);

    //        player->camera._pos = Vec3(4.22, 3.00, 2.17);
    level.player->camera._pos = Vec3(3.99, 12.37, 11.91);
    level.player->camera.rgOX = -0.69;
    level.player->camera.rgOY =  0.03;
}

void MyAppCallback::on_mousedown(){
    level.bullets.fire(level.player->camera);
}

void MyAppCallback::on_keydown(SDL_Scancode scancode){
    switch (scancode) {
    case SDLK_ESCAPE: return;
    case SDL_SCANCODE_Z: {
        bool is_wireframe = level.scene.wireframe();
        if (is_wireframe){
            level.scene.ambient_color = Vec3(0, 0, 0, 0);
        } else {
            level.scene.ambient_color = Vec3(1,1,1, 1).bright_rgb(0.33);
        }
        level.scene.wireframe(!is_wireframe);
    } break;
    case SDL_SCANCODE_R: {
        //            level.explosion->particles.explode();
        //            level.smoke->particles.explode();
    } break;
    }
}

void MyAppCallback::on_tick(double tick_time){
    level.bullets.update();
    level.scene.integrate();
    level.scene.render();


    // camera navigation
    {
        Vec3 dir;
        float moving_speed = tick_time * (this->is_ctrl_pressed?6*2:6);
        if (this->keys_pressed[SDL_SCANCODE_W]){
            dir += Vec3(0, 0, -moving_speed);
            level.player->camera.go(moving_speed);
        }
        if (this->keys_pressed[SDL_SCANCODE_S]){
            dir += Vec3(0, 0, moving_speed);
            level.player->camera.go(-moving_speed);
        }
        if (this->keys_pressed[SDL_SCANCODE_A]){
            dir += Vec3(-moving_speed, 0, 0);
            level.player->camera.stride(-moving_speed);
        }
        if (this->keys_pressed[SDL_SCANCODE_D]){
            dir += Vec3(moving_speed, 0, 0);
            level.player->camera.stride(moving_speed);
        }

        if (this->keys_pressed[SDL_SCANCODE_UP]){
            dir += Vec3(0, 0, -moving_speed);
            level.enemy->camera.go(moving_speed);
            //                    level.fireball->camera.go(moving_speed);
        }
        if (this->keys_pressed[SDL_SCANCODE_DOWN]){
            dir += Vec3(0, 0, moving_speed);
            level.enemy->camera.go(-moving_speed);
            //                    level.fireball->camera.go(-moving_speed);
        }
        if (this->keys_pressed[SDL_SCANCODE_LEFT]){
            dir += Vec3(-moving_speed, 0, 0);
            level.enemy->camera.stride(-moving_speed);
            //                    level.fireball->camera.stride(-moving_speed);
        }
        if (this->keys_pressed[SDL_SCANCODE_RIGHT]){
            dir += Vec3(moving_speed, 0, 0);
            level.enemy->camera.stride(moving_speed);
            //                    level.fireball->camera.stride(moving_speed);
        }

        if (this->keys_pressed[SDL_SCANCODE_SPACE]){
            if (level.player->_on_ground) {
                level.player->g_velocity = 10;
                level.player->_on_ground = false;
            }
        }

        level.nebula->camera.rgOX += 10.0f;
        level.nebula->camera.rgOY += 10.0f;
        //                 printf("(%0.2f, %0.2f, %0.2f), %0.2f %0.2f\n", level.player->camera._pos._x, level.player->camera._pos._y, level.player->camera._pos._z, level.player->camera.rgOX, level.player->camera.rgOY);

        //                res *= camera.getMatWorldToCamera();
        //                camera._pos += res;

    }
    // --- camera navigation

}
