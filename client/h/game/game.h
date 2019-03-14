#ifndef GAME_H
#define GAME_H


typedef enum {
    UDA_BULLET = 1,
    UDA_ENEMY = 2,
    UDA_PLAYER = 3
} UDA_GROUPS;


class Enemy;
class Level;
class Bullet;

#include "game/bullet.h"
#include "game/enemy.h"
#include "game/level.h"

#endif // GAME_H
