#ifndef GEOM_H
#define GEOM_H

#include <vector>
#include "math/package.h"

struct LinePlaneIntersectionResult {
    enum class State: int { NO=0, ONE=1, MANY=2};

    LinePlaneIntersectionResult(){};
    LinePlaneIntersectionResult(State state, const Vec3 &pos) : state(state), pos(pos){};
    State state;
    Vec3 pos;
};


struct LineLineIntersectionResult {
    enum class State: int { NO=0, ONE=1, MANY=2};

    State state;
    float p1, p2;
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
    Line(){}
    Line(const Vec3 &from, const Vec3 &to);
    Line(const Plane &normal, const Vec3 &point);
    /** Center */
    Vec3 c;

    /** Shift */
    Vec3 s;

    Vec3 at(float parameter) const;

    /** Returns the parameter for a point.
     * Supposes, that the point is on the line,
     * but doesn't perform any run-time check */
    float at(const Vec3 &point) const;
    Vec3 closest(const Vec3 &point)const;
};


struct Segment {
    Line line;
    float p1;
    float p2;

    Segment(const Vec3 &A, const Vec3 &B);
    float closest_p(const Vec3 &point)const;
    Vec3 closest(const Vec3 &point)const;
};


struct Plane {
    /**
     * A plane is defined by an equation:
     * _   _                _   _________     _
     * a * x = 0    , where a = (A,B,C,D) and x = (x,y,z,1)*/
    Vec3 a;

    Vec3 normal() const { return a.xyz(1.0); }

    Plane(){}
    /** by three points / triangle **/
    Plane(const Vec3 &A, const Vec3 &B, const Vec3 &C);

    /** Can be negative */
    float distance_to(const Vec3 &point) const;
    Vec3 closest(const Vec3 &point)const;
};

struct Triangle {
    Vec3 A, B, C;
    Triangle(){};
    Triangle(const Vec3 &A, const Vec3 &B, const Vec3 &C): A(A), B(B), C(C){}

    explicit operator Plane () const {
        return Plane(A, B, C);
    }
    /** normal */
    Vec3 n() const;

    bool in_prism(const Vec3 &point) const;
    Vec3 closest(const Vec3 &point)const;
};



/*
 * TODO: visual debug of View Frustum
 * TODO: the SAME LIGHT SPHERE IN FRUSTUM, BUT FOR FUCKING COLLISIONS WITH TRIANGLES:
 *   check whether a Triangle is in a sphere

/*
 * Use cases:
 *
 * 1. For collisions:
 * d = distance(sphere, polygon)
 * if d < 0:
 *      push_away(sphere, fabs(distance) * polygon.normal
 *
 * 2. Landing detection
 * d = distance(sphere, polygon)
 * if d < 0 and
 *              angle(polygon.normal, vec3(0, 1, 0 ) < 30
 *          and
 *              # we shouldn't check distance, since the sphere has already
 *              # checked it
 *              intersects(line_down, polygon):
 *
 *    y_velocity = 0
 *
 * 3. Light culling
 * Complexity?
 *
 * for object in scene:
 *     if not object.in_frustum: continue
 *
 *     for light in lights:
 *         light.bsphere.pos = light.pos
 *         light.bsphere.r = sqrt( min_light_level / (light.power * object.color.power) ):
 *
 *     # we can't move it outside of the loop, since the light's
 *     # radius depends on the object's material
 *     lights_in_frustum = [light for light in lights if INTERSECTS(light, frustum) ]
 *
 *     lights_near_object = []
 *     for light : lights_in_frustum:
 *         if DISTANCE(light.pos, model.bsphere) <
 *             lights_near_object += [ light ]

 *     # Sort by distance to player
 *     sort(lights_near_object, key=x: distance(x.pos, camera.pos) )
 *
 *     lights_to_shader = lights_near_object[:4]
 *     while(lights_near_object.size() < 4){
 *         lights_near_object.append(FakeLight() );
 *     }
 *
 * 4. Frustum culling
 *
 * for object in scene:
 *     object.in_frustum = INTERSECTS(object.bsphere, frustum)
 * */
struct Sphere {
    Vec3 C;
    float R;
};

struct Frustum {
    /** 'lr, bt, fn - left-right, bottom-top, far-near
     * 0    1    2    3    4    5    6    7
     lbf, lbn, ltf, ltn, rbf, rbn, rtf, rtn */
    Vec3 points[8];

    /**   0      1       2       3       4        5
     * left, right, bottom,    top,    far,    near */
    Plane planes[6];

    Frustum&& init(const Vec3 lbf, const Vec3 lbn, const Vec3 ltf, const Vec3 ltn,
              const Vec3 rbf, const Vec3 rbn, const Vec3 rtf, const Vec3 rtn) {
        points[0] = lbf;
        points[1] = lbn;
        points[2] = ltf;
        points[3] = ltn;
        points[4] = rbf;
        points[5] = rbn;
        points[6] = rtf;
        points[7] = rtn;

        planes[0] = Plane(points[0], points[1], points[2]);
        planes[1] = Plane(points[4], points[5], points[6]);
        planes[2] = Plane(points[0], points[1], points[4]);
        planes[3] = Plane(points[2], points[3], points[6]);
        planes[4] = Plane(points[0], points[4], points[2]);
        planes[5] = Plane(points[1], points[5], points[3]);

        return (Frustum&&)(*this);
    }
};

inline float distance(const Segment &segment, const Vec3 &point){
    return (point-segment.closest(point)).len3();
}



inline bool in(const Sphere &sphere, const Vec3 &point);
inline bool in(const Frustum &frustum, const Vec3 &point);
inline bool in(const Frustum &frustum, const Sphere &sphere);

void pull_away(const Triangle &tri, float min_distance, Vec3 &pos, bool &collisions_found);
Vec3 pull_away(const std::vector<Triangle> &mesh, Vec3 pos, float min_distance);

LinePlaneIntersectionResult intersection(const Line &line, const Plane &plane);
LineLineIntersectionResult intersection(const Line &line1, const Line &line2);
LinePlaneIntersectionResult intersection(const Line &line, const Triangle &tri);

//TODO: test 'in' function and continue implementation of the pseudocode
#endif // GEOM_H
