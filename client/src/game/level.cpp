#include "game/game.h"

void print_collides(SPNode node1, SPNode node2){
//    printf("Collision: %s and %s\n", node1->name, node2->name);
}

const char * LEVEL_CRYSTAL_TELEPORTER_BOTTOM = "Crystal Teleporter Bottom";
Level::Level() :
    _state_gameover(*this),
    _state_gameplay(*this),
    _state_current(&_state_gameplay)
    {
}


Vec3 enemy_respawn_pos(const Vec3 &player_pos){
    bool new_pos_is_ok = false;
    Vec3 result;
    while(!new_pos_is_ok){
        result._x = rand(-20, 20);
        result._y = rand(2, 10);
        result._z = rand(-20, 20);
        new_pos_is_ok = true;
        new_pos_is_ok = new_pos_is_ok && (result - player_pos).len3() > 20;
    }
    return result;
}


void Level::init(MyAppCallback *cb, int screen_width, int screen_height){
    this->_callback = cb;
    scene.init(screen_width, screen_height);

    scene.on_collision = [this](SPNode node1, SPNode node2) { this->on_collision(node1, node2); };

    scene.nodes.emplace_back(new Node{"Ground", Node::Flags::NONE, Node::PhysFlags::COLLIDE_STATIC, (Model().load("res/level/Ground.model", Texture().data("./res/level/textures/Ground Light Tex2.pvr")/*, Color(WHITE)*/))});
    //        scene.nodes.emplace_back(new Nododel().load("res/leNode::Flags::RIGID, e{(Mvel1/Ground.model", Texture().data("./res/level/textures/Ground.pvr")/*, Color(WHITE)*/))});



    scene.nodes.emplace_back(new Node{"Flower", Node::Flags::NONE, Node::PhysFlags::COLLIDE_STATIC, (Model().load("res/level/Flower.model", Texture().data("./res/level/textures/flower_lm.pvr")/*, Color(CYAN)*/).color(Vec3(0.063, 0.041, 0.402).bright_rgb(1)))});
    //        scene.nodes.emplace_back(new Nododel().load("res/leNode::Flags::RIGID, e{(Mvel1/stairs.model", Texture().data("./res/level/textures/stairs.pvr")/*, Color(WHITE)*/))});
    scene.nodes.emplace_back(new Node{"Roof", Node::Flags::NONE, Node::PhysFlags::COLLIDE_STATIC, (Model().load("res/level/Roof.model", Texture().data("./res/level/textures/TableLightMap.pvr")/*, Color(RED/PINK)*/).color(Vec3(1, 1, 1).bright_rgb(1)))});
    scene.nodes.emplace_back(new Node{"Roof top", Node::Flags::NONE, Node::PhysFlags::COLLIDE_STATIC, (Model().load("res/level/RoofTop.model", Texture().data("./res/level/textures/RoofLightMap.pvr")/*, Color(RED/PINK)*/).color(Vec3(0.319, 0, 0.003).bright_rgb(3)))});
    scene.nodes.emplace_back(new Node{"Crystal", Node::Flags::NONE, Node::PhysFlags::COLLIDE_STATIC, (Model().load("res/level/Crystal_Big.model", Texture().data("./res/level/textures/color_white.pvr")/*, Color(RED/PINK)*/))});
    scene.nodes.emplace_back(new Node{"Crystal", Node::Flags::NONE, Node::PhysFlags::COLLIDE_STATIC, (Model().load("res/level/Crystal_Top.model", Texture().data("./res/level/textures/color_white.pvr")/*, Color(RED/PINK)*/))});
    scene.nodes.emplace_back(new Node{LEVEL_CRYSTAL_TELEPORTER_BOTTOM, Node::Flags::NONE, Node::PhysFlags::COLLIDE_STATIC, (Model().load("res/level/CrystalBottom.model", Texture().data("./res/level/textures/color_white.pvr")/*, Color(RED/PINK)*/))});
    scene.nodes.emplace_back(new Node{"BallLight1", Node::Flags::NONE, Node::PhysFlags::COLLIDE_STATIC, (Model().load("res/level/BallLight1.model", Texture().data("./res/level/textures/color_white.pvr")/*, Color(RED/PINK)*/))});
    scene.nodes.emplace_back(new Node{"BallLight2", Node::Flags::NONE, Node::PhysFlags::COLLIDE_STATIC, (Model().load("res/level/BallLight2.model", Texture().data("./res/level/textures/color_white.pvr")/*, Color(RED/PINK)*/))});
    scene.nodes.emplace_back(new Node{"BallLight3", Node::Flags::NONE, Node::PhysFlags::COLLIDE_STATIC, (Model().load("res/level/BallLight3.model", Texture().data("./res/level/textures/color_white.pvr")/*, Color(RED/PINK)*/))});
    scene.nodes.emplace_back(new Node{"BallLight4", Node::Flags::NONE, Node::PhysFlags::COLLIDE_STATIC, (Model().load("res/level/BallLight4.model", Texture().data("./res/level/textures/color_white.pvr")/*, Color(RED/PINK)*/))});

    scene.nodes.emplace_back(new Node{"Stairs", Node::Flags::NONE, Node::PhysFlags::COLLIDE_STATIC, (Model().load("res/level/Stairs1.model", Texture().data("./res/level/textures/stairs_lightmap1.pvr")/*, Color(RED/PINK)*/))});
    scene.nodes.emplace_back(new Node{"Stairs", Node::Flags::NONE, Node::PhysFlags::COLLIDE_STATIC, (Model().load("res/level/Stairs2.model", Texture().data("./res/level/textures/stairs_lightmap2.pvr")/*, Color(RED/PINK)*/))});
    scene.nodes.emplace_back(new Node{"Stairs", Node::Flags::NONE, Node::PhysFlags::COLLIDE_STATIC, (Model().load("res/level/Stairs3.model", Texture().data("./res/level/textures/stairs_lightmap3.pvr")/*, Color(RED/PINK)*/))});
    scene.nodes.emplace_back(new Node{"Stairs", Node::Flags::NONE, Node::PhysFlags::COLLIDE_STATIC, (Model().load("res/level/Stairs4.model", Texture().data("./res/level/textures/stairs_lightmap4.pvr")/*, Color(RED/PINK)*/))});
    scene.nodes.emplace_back(new Node{"Sphere", Node::Flags::NONE, Node::PhysFlags::COLLIDE_STATIC, (Model().load("res/debug/sphere_r1.model", Texture().data("./res/debug/grid.pvr")/*, Color(RED/PINK)*/))});

    nebula = make_shared<Node>();
    nebula->name = "Nebula";
    nebula->phys = Node::PhysFlags::GHOST;
    nebula->flags = Node::Flags::NONE;
    nebula->model = std::move(Model().load("res/level/Nebula_Sky.model", Texture().data("./res/level/textures/nebula.pvr")));
    scene.nodes.emplace_back(nebula);

    SPNode riffle ( new Node{"Riffle", Node::Flags::SCREENCOORDS, Node::PhysFlags::GHOST, (Model().load("res/hero/riffle_scope.model", Texture().data("./res/level/textures/color_white.pvr")/*, Color(RED/PINK)*/)).color(Vec3(0,0,1,0.2))} );
    float riffle_size = 0.17;
    riffle->camera._scale = Vec3(riffle_size, riffle_size, 1);
    scene.nodes.emplace_back(riffle);

    this->gameover_black_overlap = shared_ptr<Node>( new Node{"Game Over Black Overlap", Node::Flags::SCREENCOORDS, Node::PhysFlags::GHOST, (Model().load("res/ui/gameover_black_background.model", Texture().data("./res/colors/black.pvr")/*, Color(RED/PINK)*/)).color(Vec3(1,1,1,1))} );
    scene.nodes.emplace_back(gameover_black_overlap);
    this->gameover_black_overlap->visible = false;

    this->gameover = shared_ptr<Node>( new Node{"Game Over", Node::Flags::SCREENCOORDS, Node::PhysFlags::GHOST, (Model().load("res/ui/gameover.model", Texture().data("./res/level/textures/color_white.pvr")/*, Color(RED/PINK)*/)).color(Vec3(1,1,1,1))} );
    gameover->visible = false;
//    float riffle_size = 0.17;
//    riffle->camera._scale = Vec3(riffle_size, riffle_size, 1);
    scene.nodes.emplace_back(gameover);
//    riffle->camera._pos._z = -0.5;
//    riffle->camera.rgOY = M_PI;
//    riffle->camera._scale = Vec3(screen_height, screen_height,screen_height);

#ifndef NDEBUG
    Node *node = new Node{"Axes", Node::Flags::SCREENCOORDS, Node::PhysFlags::COLLIDE_STATIC, (Model().load("res/debug/axes.model", Texture().data("./res/debug/axes.pvr")/*, Color(RED/PINK)*/))};
//    node->camera._pos._z = -4;
//    node->camera._pos._x = -2.5;
//    node->camera._pos._y = -2.5;
    node->camera._pos._z = -0.9;
    node->camera._pos._x = -0.9;
    node->camera._pos._y = -0.9;
    node->camera._scale = Vec3(0.1, 0.1, 0.1);
    scene.nodes.emplace_back(node);

    Node *node2 = new Node{"Axes", Node::Flags::DO_NOT_TRANSFORM, Node::PhysFlags::COLLIDE_STATIC, (Model().load("res/debug/axes.model", Texture().data("./res/debug/axes.pvr")/*, Color(RED/PINK)*/))};
    this->axes = shared_ptr<Node>(node2);
    node2->camera._pos._y = 4;
    scene.nodes.emplace_back(axes);
#endif

//    ene

    enemies.resize(this->max_enemies);
    for (Enemy &enemy : this->enemies){
        enemy.is_disabled = false;
        enemy.init(this->scene);
        enemy._enemy->visible = false;
    }

    bullets.init(scene);

    player = make_shared<Node>();
    player->name = "Player";
    player->flags = Node::Flags::NONE;
    player->phys = Node::PhysFlags::COLLIDE_DYNAMIC | Node::PhysFlags::GRAVITY | Node::PhysFlags::PULL_AWAY;
//    player->radius = 0.90 * 1.0f;
    player->radius = 0.90 * 1.0f;
    //        player->camera._pos = Vec3(0, 2, 0);
    player->uda_group = UDA_PLAYER;

    scene.nodes.emplace_back(player);
    scene._camera = player;



    _state_current->on_enter();

}

void Level::on_collision(SPNode &node1, SPNode &node2)
{
    // crystal teleporter
    SPNode non_crystal_bottom, crystal_bottom;
    if (node1->name == LEVEL_CRYSTAL_TELEPORTER_BOTTOM || node2->name == LEVEL_CRYSTAL_TELEPORTER_BOTTOM){
        if(node1->name == LEVEL_CRYSTAL_TELEPORTER_BOTTOM){
            non_crystal_bottom = node2;
            crystal_bottom = node1;
        } else {
            non_crystal_bottom = node1;
            crystal_bottom = node2;
        }

        if (non_crystal_bottom == player || non_crystal_bottom->uda_group == UDA_ENEMY) {
            non_crystal_bottom->camera._pos._y += 12;
            return;
        }
    }

    if (node1 == enemy._enemy || node2 == enemy._enemy){
//        printf("Collision: %s, %s", node1->name, node2->name);
    }

    if ( node1->uda_group == UDA_BULLET && node2->uda_group == UDA_BULLET ){
        int idx_bullet1 = bullets.find(node1);
        Bullet &bullet = bullets._bullets[idx_bullet1];

        if (!bullet._is_exploded){
            bullet._explode();
            int idx_bullet2 = bullets.find(node2);
            bullets._bullets[idx_bullet1]._explode();
        }
//        bullets._bullets[idx_bullet1]._explode();
//        bullets._bullets[idx_bullet2]._explode();
    }

    if ( (node1->uda_group == UDA_ENEMY && node2->uda_group == UDA_BULLET )
         ||
         (node1->uda_group == UDA_BULLET && node2->uda_group == UDA_ENEMY )){
//        printf("Collision: %s, %s", node1->name, node2->name);
        SPNode _bullet_node, _enemy_node;
        if (node1->uda_group == UDA_BULLET){
            _bullet_node = node1;
            _enemy_node = node2;
        } else{
            _bullet_node = node2;
            _enemy_node = node1;
        }

        int idx_bullet = bullets.find(_bullet_node);

        Bullet &bullet = bullets._bullets[idx_bullet];
        if (! bullet._is_exploded && bullet._uda_who != UDA_ENEMY){
            bullet._explode();

            _enemy_node->camera._pos = enemy_respawn_pos(player->camera._pos);

            // adding new enemy with 1/2 chance
            if (this->num_enemies + 1 < this->enemies.size() && rand(1, 100) > 50){
                this->num_enemies += 1;
                Enemy &enemy2 = this->enemies[this->num_enemies-1];
                enemy2.is_disabled = false;
                enemy2._enemy->visible = true;
                enemy2._enemy->camera._pos = enemy_respawn_pos(player->camera._pos);
            }

        }
        return;
    }

    if ( (node1->uda_group == UDA_PLAYER && node2->uda_group == UDA_BULLET )
         ||
         (node1->uda_group == UDA_BULLET && node2->uda_group == UDA_PLAYER )){
//        printf("Collision: %s, %s", node1->name, node2->name);

        SPNode _bullet, _player;
        if (node1->uda_group == UDA_BULLET){
            _bullet = node1;
            _player = node2;
        } else{
            _bullet = node2;
            _player = node1;
        }

        int idx_bullet = bullets.find(_bullet);
        if (bullets._bullets[idx_bullet]._is_exploded){
            this->_change_state(this->_state_gameover);
        }
//        _player->camera._pos. = Vec3(rand(-20, 20), rand(2, 10), rand(-20, 20));
        return;
    }


    fflush(stdout);

    bullets.on_collision(node1, node2);
}

void MyAppCallback::on_mousemove(double dx, double dy){
    level.player->camera.turn_left(-dy);
    level.player->camera.turn_up(-dx);
}

void MyAppCallback::on_mousewheel(double d){
    //        s += d / 10.f;
}

void MyAppCallback::on_after_init(){
    level.init(this, this->screen_width, this->screen_height);
}

void MyAppCallback::on_mousedown(){
    level.bullets.fire(level.player->camera, UDA_PLAYER);
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

Vec3 pull_away(const Vec3 &pos, const Vec3 &from){
    Vec3 dir = from - pos;
    float d = dir.len3();
    return dir * (1.0f / (sqrt(sqrt(sqrt(d)))));
}

Vec3 walk_around(const Vec3 &pos, const Ball &ball){
    Vec3 dir = ball.C - pos;
    dir._y = 0;
    float d = dir.len3();

    if (d > ball.R) return Vec3(0,0,0);
    return -dir.normed();
}

void enemy_follows(double tick_time, Enemy &enemy, const SPNode &player){

    auto &_last_pos = enemy._last_pos;

    float enemy_velocity = 6;
    Vec3 &enemy_pos = enemy._enemy->camera._pos;
    Vec3 &player_pos = player->camera._pos;
    Vec3 dir_enemy_to_player = (player->camera._pos - enemy_pos).normed();
    Vec3 dir_enemy_to_teleport = (-enemy_pos).normed();
    if (player_pos._y - enemy_pos._y >= 7 && enemy_pos._y < 8){
        // the player is too high AND the bot is not on top - need to teleport
        enemy_pos += dir_enemy_to_teleport * enemy_velocity * tick_time;
    } else {
        Vec3 correction(0,0,0);

        if (player_pos._y > 8) {
            // the player is on top, need to play diferent
            if (enemy_pos._y > 9.5){
                // the enemy is on the rose
//                Vec3 correction = pull_away(enemy_pos + dir_enemy_to_player, Vec3(10,12.465, -10));
//                correction += pull_away(enemy_pos + dir_enemy_to_player, Vec3(-10,12.465, -10));
                correction = walk_around(enemy_pos + dir_enemy_to_player, Ball{Vec3(10,12.465, -10), 13});
                correction += walk_around(enemy_pos + dir_enemy_to_player, Ball{Vec3(-10,12.465, 10), 13});
    //            correction = correction * 6 * tick_time;
            } else if (enemy_pos._y > 8){
                // the enemy is on the red circle
//                correction = walk_around(enemy_pos + dir_enemy_to_player, Ball{Vec3(0,12.465, 0), 13});
                correction = walk_around(enemy_pos + dir_enemy_to_player, Ball{Vec3(0,12.465, 0), 14});
//                correction = walk_around(enemy_pos + dir_enemy_to_player, Ball{Vec3(0,-1, 0), 15.5});
            }
        }
        dir_enemy_to_player += correction;
        dir_enemy_to_player = dir_enemy_to_player.normed();
        enemy_pos += dir_enemy_to_player * enemy_velocity * tick_time;

        _last_pos.push_back(enemy_pos);
        constexpr int max_poses = 60;
        if (_last_pos.size() > max_poses){
            _last_pos.pop_front();
        }

        Vec3 mean;
        for (const auto &vec : _last_pos) mean += vec;
        mean = mean * (1.0f / static_cast<float>(max_poses));
        float max_deviation = 0;
        for (const auto &vec : _last_pos){
            max_deviation = max(max_deviation, (vec-mean).len3());
        }

        if (max_deviation < 10*tick_time && _last_pos.size() >= max_poses && enemy._enemy->_on_ground){
            enemy._enemy->g_velocity = 10;
            enemy._enemy->_on_ground = false;
            Vec3 &pl_pos = player->camera._pos;
            printf("Char pos: %0.2f, %0.2f, %0.2f\n", pl_pos._x, pl_pos._y, pl_pos._z);
        }


//        прыжки делать по среднему: т.е. когда отклонение (максимум расстояния) от среднего выше заданного, тогда прыгать.
//        Сейчас алгоритм считает сумму пройденных расстояний. Это неправильно. Кажется, очень часто алгоритм "тупит" из-за того,
//        что сначала идёт в одну сторону, а потом - в другую. Таким образом, я не учитываю то, что алгоритм может "мельтешить".

//        float dist = 0;
//        for (int i = 0; i < _last_pos.size()-1; i++){
//            dist += (_last_pos[i] - _last_pos[i+1]).len3();
//        }
//        dist /= static_cast<float>(max_poses);

//        if (dist < 2.5*tick_time && _last_pos.size() >= max_poses && enemy._enemy->_on_ground){
//            enemy._enemy->g_velocity = 10;
//            enemy._enemy->_on_ground = false;
//            Vec3 &pl_pos = player->camera._pos;
//            printf("Char pos: %0.2f, %0.2f, %0.2f\n", pl_pos._x, pl_pos._y, pl_pos._z);
//        }
    }
}




void MyAppCallback::on_tick(double tick_time){
    level.enemy.on_tick(tick_time);
    level.bullets.update();
    level.scene.integrate();
    level.scene.render();

    for (int i = 0; i < this->level.num_enemies; i++){
        Enemy &enemy = this->level.enemies[i];
        if (enemy.is_disabled) continue;  // redundant
//        break;
//        if (!enemy.is_disabled) break;

        enemy_follows(tick_time, enemy, level.player);

        if (rand(1, 10000) > (10000 - 200/(this->level.max_enemies - this->level.num_enemies))){
            enemy._enemy->camera.look_at(level.player->camera._pos);
            level.bullets.fire(enemy._enemy->camera, UDA_ENEMY);
        }
    }

    for (int i = 0; i < this->level.num_enemies; i++){
        Enemy &enemy1 = this->level.enemies[i];
        const Vec3 &pos1 = enemy1._enemy->camera._pos;
        Vec3 pull_dir(0,0,0,0);
        for (int j = 0; j < this->level.num_enemies; j++){
            if (i == j) continue;
            Enemy &enemy2 = this->level.enemies[j];
            Vec3 &pos2 = enemy2._enemy->camera._pos;

            Vec3 dir = (pos1-pos2);
            float d = dir.len3();
            d *= d;
            d *= d;

            pull_dir += dir * (1.0f / d) * tick_time * 30;
        }

        enemy1._enemy->camera._pos += pull_dir;
    }

#ifndef NDEBUG
     level.axes->camera.rgOX = level.player->camera.rgOX;
     level.axes->camera.rgOY = level.player->camera.rgOY;
#endif
//    level.enemy._enemy->camera.
//    level.player->camera.look_at(Vec3(0,3,0)); // level.player->camera._pos);
//    level.player->camera.look_at(level.enemy._enemy->camera._pos); // level.player->camera._pos);

//    level.enemy._enemy->camera.look_at(Vec3(0,5,0)); // level.player->camera._pos);

    // camera navigation

    level.nebula->camera.rgOX += 0.02*tick_time;
    level.nebula->camera.rgOY += 0.02*tick_time;
    // --- camera navigation

    level._state_current->on_tick(tick_time);

}

_GameOverLevelState::_GameOverLevelState(Level &level): LevelState(level){}

void _GameOverLevelState::on_tick(double tick_time)
{
    if (this->level._callback->keys_pressed[SDL_SCANCODE_KP_ENTER]){
        this->level._change_state(this->level._state_gameplay);
    }
}

void _GameOverLevelState::on_enter() {
    this->level.gameover_black_overlap->visible = true;
    this->level.gameover->visible = true;
}

void _GameOverLevelState::on_exit() {
    this->level.gameover->visible = false;
    this->level.gameover_black_overlap->visible = false;
}

_GamePlayLevelState::_GamePlayLevelState(Level &level): LevelState(level){}

void _GamePlayLevelState::on_enter() {
    this->level.player->camera.turn_up(0.5 * M_PI);
    this->level.player->camera.fly(1.001);  // TODO: remove this ducktape
    this->level.player->camera.turn_up(-1. * M_PI);

    this->level.player->camera._pos = Vec3(3.99, /*12.37*/ 26, 11.91);
    this->level.player->camera.rgOX = -0.69;
    this->level.player->camera.rgOY =  0.03;

    this->level.player->g_velocity = 0;


//    this->level.max_enemies = 10;
    this->level.num_enemies = 1;
    for (Enemy &enemy : this->level.enemies){
        enemy._enemy->camera._pos = enemy_respawn_pos(this->level.player->camera._pos);
        enemy.is_disabled = true;
        enemy._enemy->visible = false;
        enemy._enemy->camera._pos._y = 30;
    }
    for (int i = 0; i < this->level.num_enemies; i++){
        Enemy &enemy = this->level.enemies[i];
        enemy._enemy->visible = true;
        enemy.is_disabled = false;
    }

    this->level.scene._elapsed.reset();  // bugfix: without it, during slow loading, the character immediately goes throw the floor
}

void _GamePlayLevelState::on_exit() {
}

void _GamePlayLevelState::on_tick(double tick_time) {
    if (this->level.player->camera._pos._y < -1) {
        this->level._change_state(this->level._state_gameover);
    }

    MyAppCallback &cb = *this->level._callback;
    float moving_speed = tick_time * (cb.is_ctrl_pressed?6*2:6);
    if (cb.keys_pressed[SDL_SCANCODE_W]) this->level.player->camera.go(moving_speed);
    if (cb.keys_pressed[SDL_SCANCODE_S]) this->level.player->camera.go(-moving_speed);
    if (cb.keys_pressed[SDL_SCANCODE_A]) this->level.player->camera.stride(-moving_speed);
    if (cb.keys_pressed[SDL_SCANCODE_D]) this->level.player->camera.stride(moving_speed);

//        if (this->keys_pressed[SDL_SCANCODE_UP]) level.enemy->camera.go(moving_speed);
//        if (this->keys_pressed[SDL_SCANCODE_DOWN]) level.enemy->camera.go(-moving_speed);
//        if (this->keys_pressed[SDL_SCANCODE_LEFT]) level.enemy->camera.stride(-moving_speed);
//        if (this->keys_pressed[SDL_SCANCODE_RIGHT]) level.enemy->camera.stride(moving_speed);

    if (cb.keys_pressed[SDL_SCANCODE_SPACE]){
        if (this->level.player->_on_ground) {
            this->level.player->g_velocity = 10;
            this->level.player->_on_ground = false;
        }
    }

    // printf("(%0.2f, %0.2f, %0.2f), %0.2f %0.2f\n", level.player->camera._pos._x, level.player->camera._pos._y, level.player->camera._pos._z, level.player->camera.rgOX, level.player->camera.rgOY);
}
