#ifndef PACKAGE_H
#define PACKAGE_H

//#include "stdafx.h"
#include "exceptions.h"
#include <memory.h>
#include <math.h>
#include <limits.h>

constexpr float epsilon = 1e-6;

class Mat4x4;
class Vec3;

class Line;
struct Line;
struct Plane;

struct LinePlaneIntersectionResult;
struct LineLineIntersectionResult;
struct Segment;
struct Plane;
struct Triangle;
struct Ball;
struct Frustum;


#include "math/vec4.h"
#include "math/matrix.h"
#include "math/geom.h"
#include "math/random.h"

#endif // PACKAGE_H
