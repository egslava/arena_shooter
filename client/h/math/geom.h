#ifndef GEOM_H
#define GEOM_H

#include "math/package.h"

struct LinePlaneIntersectionResult {
    enum class State: int { NO, ONE, MANY};

    LinePlaneIntersectionResult(){};
    LinePlaneIntersectionResult(State state, const Vec3 &pos) : state(state), pos(pos){};
    State state;
    Vec3 pos;
};

/**                                            _    _   _
 * A Line is defined by a parametric equation: x = pc + s
 *
 * _
 * x - is the result point (x,y,z), result
 * p - parameter, variable
 * _     _
 * c and s are constants for any specific line, s.w = 1, i.e. s = (some_a, some_b, some_c, 1).
 */
struct Line {
    Line(const Vec3 &from, const Vec3 &to);
    /** Center */
    Vec3 c;

    /** Shift */
    Vec3 s;
};

struct Plane {
    /**
     * A plane is defined by an equation:
     * _   _                _   _________     _
     * a * x = 0    , where a = (A,B,C,D) and x = (x,y,z,1)*/
    Vec3 a;

    /** by three points / triangle **/
    Plane(const Vec3 &A, const Vec3 &B, const Vec3 &C);
};

LinePlaneIntersectionResult intersection(const Line &line, const Plane &plane);

#endif // GEOM_H
