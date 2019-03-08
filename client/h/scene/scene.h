#ifndef SCENE_H
#define SCENE_H

#include "gapi/gapi.h"
#include "scene/camera.h"
#include <vector>
#include <memory>
#include "resources.h"
#include <chrono>
#include <functional>

using namespace std;


/** Colliding objects are moved only if both objects are RIGID */
struct Node {
    enum class PhysFlags: int {
        PHYS_NONE = 0,
        // GHOST - totally nothing, we can walk through such object
        // and they can walk through us.
        // - MESH never changes.
        // - Model Matrix can be changed/
        GHOST = 1,

        // SOLID - ground / walls. They push us away.
        // - MESH never changes
        // - Model Matrix should not be changed, since we calculate collisions
        // based on MESH.
        COLLIDE_STATIC = 2,

        // They push each other and are pushed away from SOLIDs. Players / enemies / so on.
        // - MESH never changes since we use Bounding Spheres for collisions
        // - Model Matrix can be changed
        COLLIDE_DYNAMIC = 4,
        // SOLID-SOLID - skip, SOLID-RIGID - already have a code. RIGID-SOLID - skip,
        // RIGID-RIGID - push only a half

        PULL_AWAY = 8,
        GRAVITY = 16,

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

    // particles
    bool uses_particles = false;
    bool particles_initialised = true;
    Particles particles;
    void particles_init(const Emitter &emitter, Texture &&texture);

    std::vector<Triangle> _transformed_mesh;  // used in case of both RIGID and DYNAMIC
    bool _is_transformed_mesh_dirty;
    Camera camera;
    bool visible = true;  // is used to remove/readd nodes, without their actuall adding/removal and memory management


    float g_velocity = 0;  // gravity velocity
    bool _on_ground;

//    Ball bsphere;
    float radius = 0;
    AABB _aabb;
    int is_aabb_dirty = 1;

    // UDA: user-defined attributes. Just for comfortable using of this class
    Uint8 uda_group = 0;
};

Node::Flags operator & (Node::Flags flag1, Node::Flags flag2);
Node::PhysFlags operator & (Node::PhysFlags flag1, Node::PhysFlags flag2);
Node::PhysFlags operator | (Node::PhysFlags flag1, Node::PhysFlags flag2);
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
        this->_elapsed /= 1000000000.0;
        this->_time_last_update = std::chrono::high_resolution_clock::now();
    }
};

struct Scene {
    Scene():ambient_color(0,0,0,0){}

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
    void _undirty_aabb();
    void _move_colliding();
    void _update_particles();
    void render();

    /** You can not change the position of the Node in this callback,
     *  since this callback is called before a node is 'pulled' away from a wall
     */
    std::function<void(SPNode, SPNode)> on_collision;


    Vec3 ambient_color;
    SPNodes nodes;  // but models should have their bounding spheres and so on
};



#endif // SCENE_H
