#ifndef SCENE_H
#define SCENE_H

#include "gapi/gapi.h"
#include "scene/camera.h"
#include <vector>
#include <memory>
#include "resources.h"
#include <chrono>

using namespace std;


/** Colliding objects are moved only if both objects are RIGID */
struct Node {
    enum class PhysFlags: int {
        // GHOST - totally nothing, we can walk through such object
        // and they can walk through us.
        // - MESH never changes.
        // - Model Matrix can be changed/
        GHOST = 0,

        // SOLID - ground / walls. They push us away.
        // - MESH never changes
        // - Model Matrix should not be changed, since we calculate collisions
        // based on MESH.
        SOLID = 1,

        // They push each other and are pushed away from SOLIDs. Players / enemies / so on.
        // - MESH never changes since we use Bounding Spheres for collisions
        // - Model Matrix can be changed
        RIGID = 2
        // SOLID-SOLID - skip, SOLID-RIGID - already have a code. RIGID-SOLID - skip,
        // RIGID-RIGID - push only a half

    };
    enum class Flags : int {
        NONE = 0,
        INVISIBLE = 1, // the model should not be rendered. Still collideable
        GONE = 2,  // the same meaning like in Android SDK: the node should not be taken into account at all
        LIGHT = 4
    };
    const char *name;
    Flags flags;
    PhysFlags phys;
    Model model;
    std::vector<Triangle> _transformed_mesh;  // used in case of both RIGID and DYNAMIC
    bool _is_transformed_mesh_dirty;
//    Sphere bsphere;
    Camera camera;
    bool visible;  // used to remode/readd nodes, without their actuall adding/removal and memory management
};

Node::Flags operator & (Node::Flags flag1, Node::Flags flag2);
//bool operator  !(Node::Flags flag){ return flag != Node::Flags::NONE; };


using SPNode = shared_ptr<Node>;
using SPNodes = vector<SPNode>;

struct ElapsedTime {
    using _Timer = std::chrono::high_resolution_clock;
    _Timer::time_point _time_last_update = _Timer::now();

    operator double () const{
        return this->_elapsed;
    }
    double _elapsed = 0;
    void update(){
        this->_elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(_Timer::now() - _time_last_update).count();
        this->_elapsed /= 10e9;
        this->_time_last_update = std::chrono::high_resolution_clock::now();
    }
};

struct Scene {
    ElapsedTime _elapsed;  // the time between 2 render calls

    SPNode _camera;
    Program program;

    void init();
    bool in_frustum(SPNode sp_node){
        return true;
    }

    Model _bsphere;
    bool _boundings = true;  // should render bounding boxes/spheres/so on?
    void integrate();
    bool wireframe() const;
    void wireframe(bool wireframe) const;
    void _gravity_pass(double dt);
    void _move_colliding();
    void render();

    Vec3 ambient_color;
    SPNodes nodes;  // but models should have their bounding spheres and so on
};



#endif // SCENE_H
