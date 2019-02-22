#include "math/math.h"
float rand(float min, float max){
    double r = static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
    return (r*(max-min)) + min;
}

Vec3 rand(const Ball &uniform){
    return Vec3(
                rand(-uniform.R, uniform.R) + uniform.C._x,
                rand(-uniform.R, uniform.R) + uniform.C._y,
                rand(-uniform.R, uniform.R) + uniform.C._z,
                1);
}
