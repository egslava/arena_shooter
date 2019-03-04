#include "game/level.h"

void print_collides(SPNode node1, SPNode node2){
//    printf("Collision: %s and %s\n", node1->name, node2->name);
}

void Level::init(){
    scene.init();
    scene.on_collision = print_collides;
    scene.nodes.emplace_back(new Node{"Ground", Node::Flags::NONE, Node::PhysFlags::SOLID, (Model().load("res/level/Ground.model", Texture().data("./res/level/textures/Ground Light Tex2.pvr")/*, Color(WHITE)*/))});
    //        scene.nodes.emplace_back(new Nododel().load("res/leNode::Flags::RIGID, e{(Mvel1/Ground.model", Texture().data("./res/level/textures/Ground.pvr")/*, Color(WHITE)*/))});

    nebula = make_shared<Node>();
    nebula->name = "Nebula";
    nebula->phys = Node::PhysFlags::GHOST;
    nebula->flags = Node::Flags::NONE;
    nebula->model = std::move(Model().load("res/level/Nebula_Sky.model", Texture().data("./res/level/textures/nebula.pvr")));
    scene.nodes.emplace_back(nebula);


    scene.nodes.emplace_back(new Node{"Flower", Node::Flags::NONE, Node::PhysFlags::SOLID, (Model().load("res/level/Flower.model", Texture().data("./res/level/textures/flower_lm.pvr")/*, Color(CYAN)*/).color(Vec3(0.063, 0.041, 0.402).bright_rgb(1)))});
    //        scene.nodes.emplace_back(new Nododel().load("res/leNode::Flags::RIGID, e{(Mvel1/stairs.model", Texture().data("./res/level/textures/stairs.pvr")/*, Color(WHITE)*/))});
    scene.nodes.emplace_back(new Node{"Roof", Node::Flags::NONE, Node::PhysFlags::SOLID, (Model().load("res/level/Roof.model", Texture().data("./res/level/textures/TableLightMap.pvr")/*, Color(RED/PINK)*/).color(Vec3(1, 1, 1).bright_rgb(1)))});
    scene.nodes.emplace_back(new Node{"Roof top", Node::Flags::NONE, Node::PhysFlags::SOLID, (Model().load("res/level/RoofTop.model", Texture().data("./res/level/textures/RoofLightMap.pvr")/*, Color(RED/PINK)*/).color(Vec3(0.319, 0, 0.003).bright_rgb(3)))});
    scene.nodes.emplace_back(new Node{"Crystal", Node::Flags::NONE, Node::PhysFlags::SOLID, (Model().load("res/level/Crystal_Big.model", Texture().data("./res/level/textures/color_white.pvr")/*, Color(RED/PINK)*/))});
    scene.nodes.emplace_back(new Node{"Crystal", Node::Flags::NONE, Node::PhysFlags::SOLID, (Model().load("res/level/Crystal_Top.model", Texture().data("./res/level/textures/color_white.pvr")/*, Color(RED/PINK)*/))});
    scene.nodes.emplace_back(new Node{"Crystal", Node::Flags::NONE, Node::PhysFlags::SOLID, (Model().load("res/level/CrystalBottom.model", Texture().data("./res/level/textures/color_white.pvr")/*, Color(RED/PINK)*/))});
    scene.nodes.emplace_back(new Node{"BallLight1", Node::Flags::NONE, Node::PhysFlags::SOLID, (Model().load("res/level/BallLight1.model", Texture().data("./res/level/textures/color_white.pvr")/*, Color(RED/PINK)*/))});
    scene.nodes.emplace_back(new Node{"BallLight2", Node::Flags::NONE, Node::PhysFlags::SOLID, (Model().load("res/level/BallLight2.model", Texture().data("./res/level/textures/color_white.pvr")/*, Color(RED/PINK)*/))});
    scene.nodes.emplace_back(new Node{"BallLight3", Node::Flags::NONE, Node::PhysFlags::SOLID, (Model().load("res/level/BallLight3.model", Texture().data("./res/level/textures/color_white.pvr")/*, Color(RED/PINK)*/))});
    scene.nodes.emplace_back(new Node{"BallLight4", Node::Flags::NONE, Node::PhysFlags::SOLID, (Model().load("res/level/BallLight4.model", Texture().data("./res/level/textures/color_white.pvr")/*, Color(RED/PINK)*/))});

    scene.nodes.emplace_back(new Node{"Stairs", Node::Flags::NONE, Node::PhysFlags::SOLID, (Model().load("res/level/Stairs1.model", Texture().data("./res/level/textures/stairs_lightmap1.pvr")/*, Color(RED/PINK)*/))});
    scene.nodes.emplace_back(new Node{"Stairs", Node::Flags::NONE, Node::PhysFlags::SOLID, (Model().load("res/level/Stairs2.model", Texture().data("./res/level/textures/stairs_lightmap2.pvr")/*, Color(RED/PINK)*/))});
    scene.nodes.emplace_back(new Node{"Stairs", Node::Flags::NONE, Node::PhysFlags::SOLID, (Model().load("res/level/Stairs3.model", Texture().data("./res/level/textures/stairs_lightmap3.pvr")/*, Color(RED/PINK)*/))});
    scene.nodes.emplace_back(new Node{"Stairs", Node::Flags::NONE, Node::PhysFlags::SOLID, (Model().load("res/level/Stairs4.model", Texture().data("./res/level/textures/stairs_lightmap4.pvr")/*, Color(RED/PINK)*/))});
    scene.nodes.emplace_back(new Node{"Sphere", Node::Flags::NONE, Node::PhysFlags::SOLID, (Model().load("res/debug/sphere_r1.model", Texture().data("./res/debug/grid.pvr")/*, Color(RED/PINK)*/))});

    player = make_shared<Node>();
    player->name = "Player";
    player->flags = Node::Flags::NONE;
    player->phys = Node::PhysFlags::RIGID;
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
