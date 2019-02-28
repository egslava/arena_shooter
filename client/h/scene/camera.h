#ifndef CAMERA_H
#define CAMERA_H

#include "math/math.h"

class Camera{
    Mat4x4 mat = Mat4x4::I;

    Mat4x4 rotW2C()const {
        // Please, pay attention, that we rotate THE WORLD, first, around Y, then, around X.
        // thus, Y doesn't change its place and always looks up
        // it can be unintuitive and contraversal to Wikipedia "General Rotations" formula[1]_
        // but it's the way it should be done, since we don't rotate an object,
        // but doing a reverse transform in a reverse order (rotating a world).
        // You can get more understanding if you try just to turn camera up and, then,
        // turn camera right.
        //
        // .. [1] https://en.wikipedia.org/wiki/Rotation_matrix
        //
        return
                Mat4x4::set_rot_x(-this->rgOX) *
                Mat4x4::set_rot_y(-this->rgOY)
                ;
    }

    Mat4x4 rotC2W() const{
        return
                Mat4x4::set_rot_y(this->rgOY) *
                Mat4x4::set_rot_x(this->rgOX)
                ;
    }

public:
    Vec3 _pos;
    float rgOX=0, rgOY=0;  // rotate around global OX and OY

    Mat4x4 getMatWorldToCamera() const;
    Mat4x4 getMatCameraToWorld() const;
    void turn_up(float updown);
    void turn_left(float radians);
    void fly(float distance);
    void go(float distance);
    void stride(float distance_right);

};

#endif // CAMERA_H
