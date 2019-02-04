//#include "math/vec4.h"
#include "math/package.h"

const Vec3 Vec3::OX(1, 0, 0, 1);
const Vec3 Vec3::OY(0, 1, 0, 1);
const Vec3 Vec3::OZ(0, 0, 1, 1);

#ifndef NDEBUG
namespace vec4_tests{
#include <assert.h>
int test_baseops(){
    assert(-Vec3(1,2,3) == Vec3(-1,-2,-3));
    assert(Vec3(1,2,3) + Vec3(4,5,6) == Vec3(5,7,9));
    assert(Vec3(1,2,3) - Vec3(4,5,6) == -3*Vec3(1,1,1));

    return 0;
}

int tests(){
    test_baseops();
    return 0;
}

int _tests = tests();
}
#endif
