#include "math/math.h"

const float _dataI[] = {
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
};

const Mat4x4 Mat4x4::I(_dataI);

const float _dataO[] = {
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0
};

const Mat4x4 Mat4x4::O(_dataO);

Mat4x4 operator * (const Mat4x4 &mat1, const Mat4x4& mat2) noexcept{
    Mat4x4 result;

    for (int i = 0 ; i < Mat4x4::ROWS; i++){
        for (int j = 0; j < Mat4x4::COLS; j++){
            result.at(i,j) = 0;
            for (int k = 0; k < mat1.COLS; k++){
                result.at(i,j) += mat1(i, k) * mat2(k, j);
            }
        }
    }
    return result;
}

Vec3 operator * (const Mat4x4 &mat, const Vec3& vec4) noexcept {
    Vec3 result(0, 0, 0, 0);
    for (int i = 0 ; i < Mat4x4::ROWS; i++){
        for (int k = 0; k < mat.COLS; k++){
            result[i] += mat(i, k) * vec4(k);
        }
    }
    return result;
}

Mat4x4 &Mat4x4::operator =(const Mat4x4 &mat2) noexcept {
    memcpy(this->_data, mat2._data, sizeof(float) * ROWS * COLS);
}

bool Mat4x4::operator ==(const Mat4x4 &another) const
{
    bool result = true;
    for (int i = 0 ; i < Mat4x4::ROWS * Mat4x4::COLS; i++){
        result &= fabs(_data[i] - another._data[i]) <= epsilon;
    }
    return result;
}

Mat4x4 Mat4x4::set_rot_x(float angle) {
    float data[16] = {
        1,              0,             0,                0,
        0,     cos(angle),   -sin(angle),                0,
        0,     sin(angle),    cos(angle),                0,
        0,              0,             0,                1
    };

    Mat4x4 result(data);
    return result;
}

Mat4x4 Mat4x4::set_rot_y(float angle){
    float data[16] = {
        cos(angle),              0,    sin(angle),                0,
                 0,              1,             0,                0,
       -sin(angle),              0,    cos(angle),                0,
                 0,              0,             0,                1
    };

    Mat4x4 result(data);
    return result;
}

Mat4x4 Mat4x4::set_rot_z(float angle)
{
    float data[16] = {
         cos(angle),      sin(angle),             0,                0,
        -sin(angle),      cos(angle),             0,                0,
                  0,               0,             1,                0,
                  0,               0,             0,                1
    };

    Mat4x4 result(data);
    return result;
}

Mat4x4 Mat4x4::set_scale(const Vec3 &vec){ return Mat4x4::set_scale(vec._x, vec._y, vec._z); }
Mat4x4 Mat4x4::set_scale(float sx, float sy, float sz){
    float data[16] = {
        sx,              0,             0,                0,
         0,             sy,             0,                0,
         0,              0,            sz,                0,
         0,              0,             0,                1
    };

    Mat4x4 result(data);
    return result;
}

Mat4x4 Mat4x4::T() const noexcept{
    Mat4x4 result;
    for (int i = 0; i < ROWS; i++){
        for (int j = 0; j < COLS; j++){
            result.at(i,j) = (*this)(j,i);
        }
    }
    return result;
}

Mat4x4 Mat4x4::set_translation(float tx, float ty, float tz){
    float data[16] = {
        1,              0,             0,               tx,
        0,              1,             0,               ty,
        0,              0,             1,               tz,
        0,              0,             0,                1
    };

    Mat4x4 result(data);
    return result;
}

Mat4x4 Mat4x4::set_translation(const Vec3 &vec3)
{
    return Mat4x4::set_translation(vec3._x, vec3._y, vec3._z);
}

Mat4x4 Mat4x4::set_ortho(float left, float right, float bottom, float top, float nearVal, float farVal) {
    // thx to: https://unspecified.wordpress.com/2012/06/21/calculating-the-gluperspective-matrix-and-other-opengl-matrix-maths/
    float sx = 2. / (right - left);
    float sy = 2. / (top - bottom);
    float sz = -2. / (farVal - nearVal);
    float tx = -(right+left) / (right-left);
    float ty = -(top+bottom) / (top-bottom);
    float tz = -(farVal+nearVal) / (farVal-nearVal);

    float data[16] = {
        sx,              0,              0,                tx,
                        0,             sy,              0,                ty,
                        0,              0,             sz,                tz,
                        0,              0,              0,                1
    };

    Mat4x4 result(data);
    return result;
}

Mat4x4 Mat4x4::set_perspective(float fovy, float aspect, float zNear, float zFar) {
    // thx to: https://unspecified.wordpress.com/2012/06/21/calculating-the-gluperspective-matrix-and-other-opengl-matrix-maths/
    float f = 1.0f / tan(fovy/2.0f);
    float sx = f / aspect;
    float sy = f;
    float sz = (zFar + zNear) / (zNear - zFar);
    float tz = (2*zFar*zNear) / (zNear - zFar);

    float data[16] = {
        sx,              0,              0,                 0,
        0,             sy,              0,                 0,
        0,              0,             sz,                tz,
        0,              0,             -1,                 0
    };
    Mat4x4 result(data);
    return result;
}

/**
 * @brief Mat4x4::rand returns matrix of values between
 * @param min
 * @param max
 * @return Matrix4x4 with all values random.
 */
Mat4x4 Mat4x4::rand(float min, float max)
{
    constexpr auto LEN = Mat4x4::ROWS*Mat4x4::COLS;
    float data[LEN];
    for(int i = 0; i < LEN; i++){
        double rand = static_cast<double>(::rand()) / static_cast<double>(INT_MAX);
        data[i] = rand * (max-min) + min;
    }

    return Mat4x4(data);
}


#ifndef NDEBUG
#include <stdio.h>
#include <assert.h>
namespace matrix_tests{

const float A_data[16] = {
    1,   2,   3,   4,
    5,   6,   7,   8,
    9,  10,  11,  12,
   13,  14,  15,  16
};
const float B_data[16] = {
   16,  15,  14,  13,
   12,  11,  10,   9,
    8,   7,   6,   5,
    4,   3,   2,   1
};
const float AB_data[16] = {
   80,  70,  60,  50,
  240, 214, 188, 162,
  400, 358, 316, 274,
  560, 502, 444, 386
};
const float BA_data[16] = {
  386, 444, 502, 560,
  274, 316, 358, 400,
  162, 188, 214, 240,
   50,  60,  70,  80
};
Mat4x4 A(A_data), B(B_data), AB(AB_data), BA(BA_data);
const Vec3 a(5,   6,   7,   8);
const Vec3 Aa( 70, 174, 278, 382);
const Vec3 Ba(372, 268, 164,  60);

int test_base_props(){
    // let's test all the matrix's properties...
    assert (Mat4x4::I * Mat4x4::I == Mat4x4::I);
    assert (Mat4x4::I * Mat4x4::O == Mat4x4::O);
    assert (Mat4x4::O * Mat4x4::I == Mat4x4::O);

    Mat4x4 rand = Mat4x4::rand(-1, 1);
    assert (Mat4x4::I * rand == rand);
    assert (rand * Mat4x4::I == rand);
    assert (Mat4x4::O * rand == Mat4x4::O);
    assert (rand * Mat4x4::O == Mat4x4::O);
}
int test_mul_matmat(){
    assert(A*B == AB);
    assert(B*A == BA);
}
int test_mul_matvec(){
    assert(A*a == Aa);
    assert(B*a == Ba);
}

int tests(){
    test_base_props();
    test_mul_matmat();
    test_mul_matvec();


    const Vec3 OX(1, 0, 0, 1), OY(0, 1, 0, 1), OZ(0, 0, 1, 1);
    assert (-OZ == Mat4x4::set_rot_y(90.f / 180.f * M_PI) * OX);
    assert ( OZ == Mat4x4::set_rot_x(90.f / 180.f * M_PI) * OY);


    printf("Matrix tests as well!\n");
    return 0;
}

int __tests = tests();

}
#endif
