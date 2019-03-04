#ifndef LEVEL_H
#define LEVEL_H

#include "game/bullet.h"
#include "scene/scene.h"

struct Level {
    Scene scene;
    SPNode player, enemy, nebula;
    SPNodes enemies;
    Bullets bullets;

    void init();
};


#endif // LEVEL_H
