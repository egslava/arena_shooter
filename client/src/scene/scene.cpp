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

void Scene::init(){

    Shader vertex_shader, fragment_shader;
    vertex_shader.compile(Shader::Type::VERTEX_SHADER, vertex_shader_code);
    fragment_shader.compile(Shader::Type::FRAGMENT_SHADER, fragment_shader_code);

    program.link(std::move(vertex_shader), std::move(fragment_shader));
}

void Scene::move_colliding()
{
    float min_distance = 1.1;
    for (const auto &node1 : this->nodes){
        if (node1->phys == Node::PhysFlags::GHOST )
            continue;
        if (node1->phys == Node::PhysFlags::SOLID)
            continue;  // should never intersect or it just was designed and it's ok!
        if ((node1->flags & Node::Flags::GONE) != Node::Flags::NONE)
            continue;
        Vec3 pos = node1->camera.getMatCameraToWorld() * Vec3(0, 0, 0);

        for (const auto &node2 : this->nodes){
            if (node1 == node2)
                continue;
            if ((node2->flags & Node::Flags::GONE) != Node::Flags::NONE)
                continue;
            if (node2->phys == Node::PhysFlags::GHOST)
                continue;
//            if (node2->phys == Node::PhysFlags::SOLID)
//                continue;

            Vec3 res;
            for (const Triangle &tri: node2->model._triangles){
                Line l (pos, pos+tri.n());
                LinePlaneIntersectionResult p=intersection(l, tri);
                if (p.state != p.State::ONE) continue;

                float d = static_cast<Plane>(tri).distance_to(pos);
                float d2 = static_cast<Plane>(tri).distance_to(pos);
                // printf("line: c: %f;%f;%f, s: %f;%f;%f\n", l.c._x, l.c._y, l.c._z, l.s._x, l.s._y, l.s._z);
                // printf("intersection. x: %f, y: %f, z: %f\n", p.pos._x, p.pos._y, p.pos._z);
                // printf("triangle. %0.2f,%0.2f,%0.2f;  %0.2f,%0.2f,%0.2f;  %0.2f,%0.2f,%0.2f\n", tri.A._x, tri.A._y, tri.A._z, tri.B._x, tri.B._y, tri.B._z, tri.C._x, tri.C._y, tri.C._z);
                if (fabs(d) <= min_distance ){ //&& fabs(d) <= nearest_distance){
                    float nearest_distance = fabs(d);
                    //                        res = tri.n() * dir.len3();
                    //                        res = tri.n() * (tri.n().dot3(dir));
                    //                        res += tri.n() * (0.15f-fabs(d)) * (d<0?-1:1);
                    res += (d<0?-1:1)*tri.n() * (min_distance-fabs(d));
                }
            }

            pos = pos + res;
            node1->camera._pos = pos; //dir + res;
        }
    }
}

void Scene::render(){

    //    	program.transform(angleOY, x, y, z, s, s, s);
    program.use(_camera->camera);

    for (const SPNode &node : nodes){

        if (in_frustum(node)){
            node->model.draw(); // scene.render(node);
        }
    }
}

Node::Flags operator &(Node::Flags flag1, Node::Flags flag2){
    return static_cast<Node::Flags>(static_cast<int>(flag1) & static_cast<int>(flag2));
}
