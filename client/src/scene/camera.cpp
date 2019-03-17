#include "scene/camera.h"
#include "Config.h"
#include <math.h>

#ifndef M_PI
// strangely, on Windows, I don't have this constant
#define M_PI 3.14159265358979323846
#endif

//Mat4x4 Camera::getMat() const
//{
//    // https://en.wikipedia.org/wiki/Rotation_matrix general rotations says, that it's ok
//    return
//            Mat4x4::set_rot_x(this->rgOX) *
//            Mat4x4::set_rot_y(this->rgOY) *
//            Mat4x4::set_translation(this->x, this->y, this->z)
//                                   ;
//}

//void Camera::turn_up(float updown) {
//    this->rgOX -= updown;
//    this->rgOX = fmax(this->rgOX, -M_PI/2);
//    this->rgOX = fmin(this->rgOX,  M_PI/2);
////    this->mat = Mat4x4::set_rot_x(-updown) * this->mat;
//}

//void Camera::turn_right(float rightleft) {
//    this->rgOY -= rightleft;
////    this->mat = this->mat * Mat4x4::set_rot_y(-rightleft);
//}

//void Camera::go(float distance) {
//    Mat4x4 direction =
//            Mat4x4::set_rot_x(this->rgOX) *
//            Mat4x4::set_rot_y(this->rgOY)
//            ;
//    float dx=direction(2, 0), dy=direction(2, 1), dz=direction(2, 2);  // we're interested in -z

//    this->x += dx * distance;
//    this->y += dy * distance;
//    this->z += dz * distance;

////    this->mat = Mat4x4::set_translation(0, 0, distance) * this->mat;
//}

//void Camera::stride(float distance_right) {
//    Mat4x4 direction =
//            Mat4x4::set_rot_x(this->rgOX) *
//            Mat4x4::set_rot_y(this->rgOY)
//            ;
//    float dx=direction(0, 0), dy=direction(0, 1), dz=direction(0, 2);  // we're interested in -z

//    this->x += dx * -distance_right;
//    this->y += dy * -distance_right;
//    this->z += dz * -distance_right;

////    this->mat = Mat4x4::set_translation(-distance_right, 0, 0) * this->mat;
//}


Mat4x4 Camera::getMatWorldToCamera() const
{
    return Mat4x4::set_scale(this->_scale.invdot() ) * this->rotW2C()*Mat4x4::set_translation(-this->_pos);
}

Mat4x4 Camera::getMatCameraToWorld() const
{
    return Mat4x4::set_translation(this->_pos) * this->rotC2W() * Mat4x4::set_scale(this->_scale);
}

void Camera::turn_up(float updown) {
    this->rgOX += updown;
    this->rgOX = fmax(this->rgOX, -M_PI/2);
    this->rgOX = fmin(this->rgOX,  M_PI/2);
}

void Camera::turn_left(float radians) {
    this->rgOY += radians;
}

#include <stdio.h>
void Camera::fly(float distance) {
    Vec3 cDir(0.f, 0.f, -distance, 1);  // camera's oZ
    Vec3 wDir = this->getMatCameraToWorld() * cDir;  // world's direction
    this->_pos = wDir;
}

void Camera::go(float distance) {
    float rgOX = this->rgOX;
    this->rgOX = 0;
    fly(distance);

//    Vec3 cDir(0.f, 0.f, -distance, 1);  // camera's oZ
//    Vec3 wDir = (this->getMatCameraToWorld() * cDir) - this->_pos;  // world's direction
////    wDir._y = 0;
//    wDir *= fabs(distance) * 1.0f / wDir.len3();
//    this->_pos += wDir;
    this->rgOX = rgOX;
}

void Camera::stride(float distance_right) {
    Vec3 cNewPos(distance_right, 0.f, 0, 1);  // camera's oZ
    Vec3 wNewPos = this->getMatCameraToWorld() * cNewPos;
    this->_pos = wNewPos;
}

//#include <math.h>
void Camera::look_at(const Vec3 &point) {
    /* The basic formula can be found here: https://en.wikipedia.org/wiki/Spherical_coordinate_system
     * "Cartessian coordinates":
     *  this->rgOX = acos(y);
     *  this->rgOY = atan2(x,z);
     *  But it doesn't work properly in my case. I need to adjust it a bit:
     *  this->rgOX = -(acos(y) - M_PI/2);
     *  this->rgOY = atan2(x,z) - M_PI;
     *
     * I didn't have time to investigate, why do I need to add these fixes, but it passed 3 tests:
     * 1. A player always looks at the enemy.
     * 2. A player always looks at the origin.
     * 3. An enemy always looks at the player. Instead of enemy I put XYZ-axes model
    */

    const Vec3 wDir = (point - this->_pos).normed();
    const float &x=wDir._x, &y = wDir._y, &z = wDir._z;
    if (fabs(z) <= epsilon) return;
    this->rgOX = -(acos(y) - M_PI/2);
    this->rgOY = atan2(x,z) - M_PI;
    this->rgOZ = 0;
}

#ifdef RUN_TESTS
#include <stdio.h>
#include <assert.h>

const Vec3 OX (1, 0, 0, 1);
const Vec3 OY (0, 1, 0, 1);
const Vec3 OZ (0, 0, 1, 1);

float test_rot_right_up(){
    Camera cam;  // in 0,0,0
    cam.turn_up(   90. / 180. * M_PI);
    cam.turn_left(-90. / 180. * M_PI);
    assert(cam.getMatCameraToWorld()*OX == OZ);
    assert(cam.getMatCameraToWorld()*OY ==-OX);
    assert(cam.getMatCameraToWorld()*OZ ==-OY);
	return 0;
}
float test_doesnt_change_axes(){
    Camera cam;  // in 0,0,0
    assert ((cam.getMatWorldToCamera() * OX).eqXYZ(OX));
    assert ((cam.getMatWorldToCamera() * OY).eqXYZ(OY));
    assert ((cam.getMatWorldToCamera() * OZ).eqXYZ(OZ));


    // rot up/down
    cam.turn_up(90. / 180. * M_PI);
    assert(cam._pos == Vec3(0, 0, 0));
    assert ((cam.getMatWorldToCamera() * OX).eqXYZ(OX));
    assert ((cam.getMatWorldToCamera() * OY).eqXYZ(-OZ));
    assert ((cam.getMatWorldToCamera() * OZ).eqXYZ(OY));

    cam.turn_up(-90. / 180. * M_PI);
    cam.fly(1);  // moved by 1 in -z direction
    assert(cam._pos == Vec3(0, 0, -1));
    assert ((cam.getMatWorldToCamera() * OX).eqXYZ(Vec3(1, 0, 1)));
    assert ((cam.getMatWorldToCamera() * OY).eqXYZ(Vec3(0, 1, 1)));
    assert ((cam.getMatWorldToCamera() * OZ).eqXYZ(Vec3(0, 0, 2)));

    cam.turn_left(-90. / 180. * M_PI);
//    assert ((cam.getMatWorldToCamera() * OX).eqXYZ(OZ-OX));
    assert ((cam.getMatWorldToCamera() * OX).eqXYZ( OX - OZ));
    assert ((cam.getMatWorldToCamera() * OY).eqXYZ( OX + OY));
    assert ((cam.getMatWorldToCamera() * OZ).eqXYZ(  2 * OX));

//    cam.turn_up(90. / 180. * M_PI);
//    assert(cam._pos == Vec3(0, 0, 0));
//    assert ((cam.getMatWorldToCamera() * OX).eqXYZ(OX));
//    assert ((cam.getMatWorldToCamera() * OY).eqXYZ(-OZ));
//    assert ((cam.getMatWorldToCamera() * OZ).eqXYZ(OY));

    cam.fly(1);  // moved by 1 in x direction
    assert ((cam._pos == Vec3(1, 0, -1)));
    assert ((cam.getMatWorldToCamera() * Vec3(0,0,0)).eqXYZ(Vec3( 1, 0, 1)));
    assert ((cam.getMatWorldToCamera() * OX).eqXYZ(OX));
    assert ((cam.getMatWorldToCamera() * OY).eqXYZ(Vec3(1, 1, 1)));
    assert ((cam.getMatWorldToCamera() * OZ).eqXYZ(Vec3( 2, 0, 1)));

	return 0;
}

float tests(){
    test_doesnt_change_axes();
    test_rot_right_up();
    printf("Ha-ha! It's the test func!\n");
    return -1.0f;
}

float n = tests();
#endif
