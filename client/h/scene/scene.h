#ifndef SCENE_H
#define SCENE_H

#include "scene/model.h"
#include "scene/camera.h"
#include <vector>
#include <memory>
#include "resources.h"

using namespace std;

/** Colliding objects are moved only if both objects are RIGID */

struct Node {
    enum class PhysFlags: int {
        GHOST = 0,  // totally nothing, we can walk through such objects
        SOLID = 1,  // ground / walls. They push us away
        RIGID = 2   // they push each other
        // SOLID-SOLID - skip, SOLID-RIGID - already have a code. RIGID-SOLID - skip,
        // RIGID-RIGID - push only a half

    };
    enum class Flags : int {
        NONE = 0,
        DYNAMIC = 2,// model matrix gonna be changed (player, enemy, but not part of a level)
        INVISIBLE = 4, // the model should not be rendered. Still collideable
        GONE = 8,  // the same meaning like in Android SDK: the node should not be taken into account at all
        LIGHT = 16
    };
    const char *name;
    Flags flags;
    PhysFlags phys;
    Model model;
    std::vector<Triangle> _transformed_mesh;  // used in case of both RIGID and DYNAMIC
//    Sphere bsphere;
    Camera camera;
    bool visible;  // used to remode/readd nodes, without their actuall adding/removal and memory management
};

Node::Flags operator & (Node::Flags flag1, Node::Flags flag2);
//bool operator  !(Node::Flags flag){ return flag != Node::Flags::NONE; };


using SPNode = shared_ptr<Node>;
using SPNodes = vector<SPNode>;

struct Scene {
    SPNode _camera;
    Program program;

    void init();
    bool in_frustum(SPNode sp_node){
        return true;
    }
    void move_colliding();
    void render();

    SPNodes nodes;  // but models should have their bounding spheres and so on
};



#endif // SCENE_H
