#include "stdafx.h"
#include "scene/scene.h"
#include "math/vec4.h"

void collide(){
}

void somewhat_related_to_collisions2(){
//    Vec3 cam = camera.getMatCameraToWorld() * Vec3(0, 0, 0);
//    dir = camera.getMatCameraToWorld() * dir;
//    Vec3 res;
//    Line l (cam, dir);

//    float nearest_distance = 10000;
//    float min_distance = 0.075;
//    //                for (const Triangle &tri: models[i_active_vao]._triangles){
//    //                    LinePlaneIntersectionResult p=intersection(l, tri);
//    //                    if (p.state != p.State::ONE) continue;

//    ////                    printf("Bump!\n");

//    ////                    float d = (cam - p.pos).len3();
//    ////                    float d = static_cast<Plane>(tri).distance_to(camera._pos);
//    //                    float d = static_cast<Plane>(tri).distance_to(dir);
//    //                    float d2 = static_cast<Plane>(tri).distance_to(cam);
//    //                    // printf("line: c: %f;%f;%f, s: %f;%f;%f\n", l.c._x, l.c._y, l.c._z, l.s._x, l.s._y, l.s._z);
//    //                    // printf("intersection. x: %f, y: %f, z: %f\n", p.pos._x, p.pos._y, p.pos._z);
//    //                    // printf("triangle. %0.2f,%0.2f,%0.2f;  %0.2f,%0.2f,%0.2f;  %0.2f,%0.2f,%0.2f\n", tri.A._x, tri.A._y, tri.A._z, tri.B._x, tri.B._y, tri.B._z, tri.C._x, tri.C._y, tri.C._z);

//    //                    if (fabs(d) <= min_distance ){ //&& fabs(d) <= nearest_distance){
//    //                        nearest_distance = fabs(d);
//    ////                        res = tri.n() * dir.len3();
//    ////                        res = tri.n() * (tri.n().dot3(dir));
//    ////                        res += tri.n() * (0.15f-fabs(d)) * (d<0?-1:1);
//    //                        res = (d<0?-1:1)*tri.n() * (min_distance-fabs(d));
//    //                    }
//    //                }

//    cam = dir + res;
//    camera._pos = cam; //dir + res;



}

void Scene::init(int screen_width, int screen_height){
    this->_screen_width = screen_width;
    this->_screen_height = screen_height;

    Shader vertex_shader, fragment_shader;
    vertex_shader.compile(Shader::Type::VERTEX_SHADER, vertex_shader_code);
    fragment_shader.compile(Shader::Type::FRAGMENT_SHADER, fragment_shader_code);

    this->_bsphere.load("./res/debug/sphere_r1.model", Texture().data("./res/debug/grid.pvr"));

    program.link(std::move(vertex_shader), std::move(fragment_shader));
}

void Scene::integrate()
{
    const float n_times = 1;

    for (int i = 0; i < n_times; i++){
        this->_undirty_aabb();
        this->_gravity_pass(_elapsed / n_times );
        this->_move_colliding();
    }

    this->_update_particles();
}

void Scene::_update_particles()
{
    for (SPNode node : this->nodes){
        if (!node->uses_particles) continue;
//        node->particles.emitter.position_range.C = node->camera._pos;
        if (!node->particles_initialised){
            node->particles.init(node->camera._pos);
            node->particles_initialised = true;
        }
        node->particles.update(node->camera._pos, node->visible);
    }
}

bool Scene::wireframe() const
{
    GLint polygonMode[2];
    glGetIntegerv(GL_POLYGON_MODE, polygonMode);
    return polygonMode[0] == GL_LINE;
}

void Scene::wireframe(bool wireframe) const { glPolygonMode( GL_FRONT_AND_BACK, wireframe?GL_LINE : GL_FILL ); }

void Scene::_gravity_pass(double dt)
{
    for (const auto &node : this->nodes){
        if ((node->phys & Node::PhysFlags::GRAVITY) == Node::PhysFlags::PHYS_NONE){
            continue;
        }

        if (node->_on_ground){
            node->g_velocity = 0;
        } else {
            node->g_velocity += -9.8 * dt;
        }
        node->camera._pos += Vec3(0, node->g_velocity * dt, 0);
    }
}

void Scene::_undirty_aabb() {
    for (const auto node: this->nodes){
        bool dynamic = (node->phys & Node::PhysFlags::COLLIDE_DYNAMIC) != Node::PhysFlags::PHYS_NONE;
        if (node->is_aabb_dirty > 0 || dynamic){
            bool aabb_by_rad = (node->phys & Node::PhysFlags::AABB_BY_RAD) != Node::PhysFlags::PHYS_NONE;
            if (node->model._triangles.size() <= 0 || aabb_by_rad) {
                Ball bsphere;
                bsphere.C = node->camera._pos;
                bsphere.R = node->radius;

                node->_aabb.set(bsphere);
            } else {
                node->_aabb.set(node->model._triangles);
            }
            node->is_aabb_dirty = max(node->is_aabb_dirty-1, 0);
        }
    }
}

//void Scene::_move_colliding()
//{
//    float min_distance = 1.1;

//    for (const auto &node1 : this->nodes){
//        if (node1->phys == Node::PhysFlags::GHOST )
//            continue;
//        if (node1->phys == Node::PhysFlags::SOLID)
//            continue;  // should never intersect or it just was designed and it's ok!
//        if ((node1->flags & Node::Flags::GONE) != Node::Flags::NONE)
//            continue;
//        Vec3 pos = node1->camera.getMatCameraToWorld() * Vec3(0, 0, 0);

//        for (const auto &node2 : this->nodes){
//            if (node1 == node2)
//                continue;
//            if ((node2->flags & Node::Flags::GONE) != Node::Flags::NONE)
//                continue;
//            if (node2->phys == Node::PhysFlags::GHOST)
//                continue;
////            if (node2->phys == Node::PhysFlags::SOLID)
////                continue;

//            Vec3 res;
//            for (const Triangle &tri: node2->model._triangles){
//                Line l (pos, pos+tri.n());
//                LinePlaneIntersectionResult p=intersection(l, tri);
//                if (p.state != p.State::ONE) continue;

//                float d = static_cast<Plane>(tri).distance_to(pos);
//                float d2 = static_cast<Plane>(tri).distance_to(pos);
//                // printf("line: c: %f;%f;%f, s: %f;%f;%f\n", l.c._x, l.c._y, l.c._z, l.s._x, l.s._y, l.s._z);
//                // printf("intersection. x: %f, y: %f, z: %f\n", p.pos._x, p.pos._y, p.pos._z);
//                // printf("triangle. %0.2f,%0.2f,%0.2f;  %0.2f,%0.2f,%0.2f;  %0.2f,%0.2f,%0.2f\n", tri.A._x, tri.A._y, tri.A._z, tri.B._x, tri.B._y, tri.B._z, tri.C._x, tri.C._y, tri.C._z);
//                if (fabs(d) <= min_distance ){ //&& fabs(d) <= nearest_distance){
//                    float nearest_distance = fabs(d);
//                    //                        res = tri.n() * dir.len3();
//                    //                        res = tri.n() * (tri.n().dot3(dir));
//                    //                        res += tri.n() * (0.15f-fabs(d)) * (d<0?-1:1);
//                    res += (d<0?-1:1)*tri.n()pos * (min_distance-fabs(d));
//                }
//            }

//            pos = pos + res;
//            node1->camera._pos = pos; //dir + res;
//        }
//    }
//}

void Scene::_move_colliding()
{
    Ball bsphere;

    for (const auto &node1 : this->nodes){
	//for (int i = 0; i < this->nodes.size(); i++) {
		//const auto &node1 = this->nodes[i];

        if (node1->phys == Node::PhysFlags::GHOST )
            continue;
        if (node1->phys == Node::PhysFlags::COLLIDE_STATIC)
            continue;  // should never intersect or it just was designed and it's ok!
        if ((node1->flags & Node::Flags::GONE) != Node::Flags::NONE)
            continue;
        if (!node1->visible)
            continue;

//        if (node1->uses_particles){
//            printf("particles_initialised: %s\n", node1->name);
//        }
//        float min_distance = 0.95 * 1.0f;
        bsphere.R = node1->radius;


        node1->_on_ground = false;
        bool node1_aabb_only = (node1->phys & Node::PhysFlags::AABB_ONLY) != Node::PhysFlags::PHYS_NONE;

        for (const auto &node2 : this->nodes){
            if (node1 == node2)
                continue;
            if ((node2->flags & Node::Flags::GONE) != Node::Flags::NONE)
                continue;
            if (node2->phys == Node::PhysFlags::GHOST)
                continue;
            if (!node2->visible)
                continue;
//            if (node2->phys == Node::PhysFlags::SOLID)
//                continue;

            if ( !in(node1->_aabb, node2->_aabb) ) continue;
            // looking for collisions: enemy <-> bullet

            bool collision_found = false;

            // the new pos should be set before calling a callback
            // otherwise, the callback will not be able to change a position
            // for instance, I had a crystal-teleporter. When we collide with the
            // crystal, the position should be changed. But it was not possible,
            // because `node1->camera._pos = pos;` rejected the changes afterwards.
            node1->camera._pos = pull_away(node2->model._triangles, node1->camera._pos, node1->radius, collision_found, node1->_on_ground);

            collision_found |= (
                        (
                            node1_aabb_only ||
                            ((node2->phys & Node::PhysFlags::AABB_ONLY) != Node::PhysFlags::PHYS_NONE)
                        )
                        ||
                        (
                            node1->model._triangles.size() == 0 &&
                            node2->model._triangles.size() == 0
                        )
                    );

            if (collision_found){
                on_collision(node1, node2);
            }
        }
    }
}

void Scene::render(){
    this->_elapsed.update();
    //    	program.transform(angleOY, x, y, z, s, s, s);
    float ratio = this->_screen_width / this->_screen_height;

    glEnable(GL_DEPTH_TEST);
    program.use(_camera->camera, this->ambient_color);
    program.set_mat_projection( Mat4x4::set_perspective(90.0f / 180.0f * 3.141529, ratio, 0.001f, 900.00f) );
    program.set_mat_camera( _camera->camera.getMatWorldToCamera() );
    for (const SPNode &node : nodes){
        if (in_frustum(node)){
            if (!node->visible) continue;
            if ((node->flags & Node::Flags::SCREENCOORDS)!=Node::Flags::NONE)  // later, during second pass
                continue;

            program.set_color(node->model._color);
            bool should_apply_transforms = (node->flags & Node::Flags::DO_NOT_TRANSFORM) == Node::Flags::NONE;
            program.set_mat_model(should_apply_transforms ? node->camera.getMatCameraToWorld() : Mat4x4::I);

            node->model.draw(node->uda_group == 2); // scene.render(node);

            if (node->uses_particles){
//                glEnable(GL_BLEND);
//                glBlendFunc(GL_ONE, GL_ONE);
//                glBlendFunc(GL_DST_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                node->particles.draw(_camera->camera, this->ambient_color);
//                glDisable(GL_BLEND);
            }
//                glBlendFunc(GL_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            if (this->_boundings){
//                node->
//                _bsphere.draw();
            }
        }
    }

    glDisable(GL_DEPTH_TEST);
    program.use(_camera->camera, this->ambient_color);
    program.set_mat_camera( Mat4x4::I );
    program.set_mat_projection( Mat4x4::set_ortho(-ratio, ratio, -1, 1, -0.001f, 900.00f));
    for (const SPNode &node : nodes){
        if (!node->visible) continue;
        if ((node->flags & Node::Flags::SCREENCOORDS)==Node::Flags::NONE)  // later, during second pass
            continue;

        program.set_color(node->model._color);
        bool should_apply_transforms = (node->flags & Node::Flags::DO_NOT_TRANSFORM) == Node::Flags::NONE;
        program.set_mat_model(should_apply_transforms ? node->camera.getMatCameraToWorld() : Mat4x4::I);

        node->model.draw(); // scene.render(node);

//        if (node->uses_particles){
//            node->particles.draw(_camera->camera, this->ambient_color);
//        }
        // if (this->_boundings) node->_bsphere.draw();
    }

}

Node::Flags operator &(Node::Flags flag1, Node::Flags flag2){
    return static_cast<Node::Flags>(static_cast<int>(flag1) & static_cast<int>(flag2));
}

Node::PhysFlags operator & (Node::PhysFlags flag1, Node::PhysFlags flag2) {
    return static_cast<Node::PhysFlags>(static_cast<int>(flag1) & static_cast<int>(flag2));
}
Node::PhysFlags operator | (Node::PhysFlags flag1, Node::PhysFlags flag2) {
    return static_cast<Node::PhysFlags>(static_cast<int>(flag1) | static_cast<int>(flag2));
}
Node::Flags operator | (Node::Flags flag1, Node::Flags flag2) {
    return static_cast<Node::Flags>(static_cast<int>(flag1) | static_cast<int>(flag2));
}


void Node::particles_init(const Emitter &emitter, Texture &&texture)
{
    this->uses_particles = true;
    this->particles_initialised = false;
    this->particles.emitter = emitter;
    this->particles._tex = std::move(texture);
}
