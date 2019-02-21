//#include "math/vec4.h"
#include "math/package.h"
#include "Config.h"

const Vec3 Vec3::OX(1, 0, 0, 1);
const Vec3 Vec3::OY(0, 1, 0, 1);
const Vec3 Vec3::OZ(0, 0, 1, 1);

#ifdef RUN_TESTS
namespace vec4_tests{
#include <assert.h>
int test_baseops(){
    assert(-Vec3(1,2,3) == Vec3(-1,-2,-3));
    assert(Vec3(1,2,3) + Vec3(4,5,6) == Vec3(5,7,9));
    assert(Vec3(1,2,3) - Vec3(4,5,6) == -3*Vec3(1,1,1));

    return 0;
}

void test_cross3(){
    assert (Vec3::OX.cross3(Vec3::OY).eqXYZ(Vec3::OZ));
    assert (Vec3::OY.cross3(Vec3::OX).eqXYZ(-Vec3::OZ));
}

void test_dot4(){
    assert ( fabs(Vec3::OX.dot4(Vec3::OY) - 1) <= epsilon);
    assert ( fabs(Vec3::OZ.dot4(Vec3::OY) - 1) <= epsilon);
    assert ( fabs(Vec3::OX.dot4(Vec3::OY) - 1) <= epsilon);

    assert ( fabs(Vec3::OX.dot4(Vec3::OX) - 2) <= epsilon);
    assert ( fabs(Vec3::OY.dot4(Vec3::OY) - 2) <= epsilon);
    assert ( fabs(Vec3::OZ.dot4(Vec3::OZ) - 2) <= epsilon);

    assert ( fabs(Vec3(0,0,0,1).dot4(Vec3(0,0,0,2)) - 2) <= epsilon);
}

void test_dot3(){
    assert ( fabs(Vec3::OX.dot3(Vec3::OY)) <= epsilon);
    assert ( fabs(Vec3::OZ.dot3(Vec3::OY)) <= epsilon);
    assert ( fabs(Vec3::OX.dot3(Vec3::OY)) <= epsilon);

    assert ( fabs(Vec3::OX.dot3(Vec3::OX) - 1) <= epsilon);
    assert ( fabs(Vec3::OY.dot3(Vec3::OY) - 1) <= epsilon);
    assert ( fabs(Vec3::OZ.dot3(Vec3::OZ) - 1) <= epsilon);

    assert ( fabs(Vec3(0,0,0,1).dot3(Vec3(0,0,0,2))) <= epsilon);
}

void test_len3(){
    assert ( fabs(Vec3::OX.len3() - 1) <= epsilon);
    assert ( fabs(Vec3::OZ.len3() - 1) <= epsilon);
    assert ( fabs(Vec3::OX.len3() - 1) <= epsilon);

    assert ( fabs(Vec3(0, 0, 0, 1).len3()) <= epsilon);
    assert ( fabs(Vec3(1, 0, 1, 1).len3() - sqrt(2)) <= epsilon);
    assert ( fabs(Vec3(0, 3, 4, 1).len3() - 5) <= epsilon);
}
int tests(){
    test_baseops();
    test_cross3();
    test_dot4();
    test_dot3();
    test_len3();
    return 0;
}

int _tests = tests();
}
#endif
