#include "math/geom.h"

Plane::Plane(const Vec3 &A, const Vec3 &B, const Vec3 &C) {
    /* The plane is defined by the equation: Ax + By + Cz + D = 0
     * ABC - is always a vector perpendicular to this plane. Thus:
     * _               __   __
     * n = (A, B, C) = AB x BC
     *
     * After we find the normal vector, we need to find D component:
     * Ax + By + Cz + D = 0     =>    D = -(Ax + By + Cz)
     * Let's rewrite it in a vector form:
     *      _
     * D = -n * (x,y,z), where x,y,z - is any point. For instance, point A.
    */
    this->a = (B-A).cross3(C-B);
    this->a._w =-A.dot3(this->a);
}

Line::Line(const Vec3 &from, const Vec3 &to) {
    this->c = (to-from).xyz(0);
    this->s = from.xyz(1);
}


LinePlaneIntersectionResult intersection(const Line &line, const Plane &plane)
{
    /* The explanation of math behind it: we need to solve the system of equations: plane and line
     * to find the intersection.
     *
     *                                             _    _   _
     * A Line is defined by a parametric equation: x = pc + s
     *                                    _   _
     * A plane is defined by an equation: a * x = 0    , x = (x,y,z,1)
     *
     *
     * _     _   _                 _    _   _ _                  _ _    _ _                  _ _   _ _
     * a * (pc + s) = 0     =>     a * pc + a*s) = 0    =>    p* a c = -a*s     =>     p = - a c / a*s
     *
     */

    LinePlaneIntersectionResult result;


    float dividend = - plane.a. dot4 (line.s) ;
    float divisor =    plane.a. dot4 (line.c) ;

    if (fabs(divisor) <= epsilon){
        if (fabs(dividend) <= epsilon){
            // 0/0 ambiguity, means, that the line lays in the plane
            result.state = LinePlaneIntersectionResult::State::MANY;
        } else {
            // number/0 = inf - wrong expression. There's no solution.
            // It means, that the line is parallel to the plane and they have 0 common points
            result.state = LinePlaneIntersectionResult::State::NO;
        }
    } else {
        result.state = LinePlaneIntersectionResult::State::ONE;
        float p = dividend / divisor;  // p - is a parameter of parametric line equastion
        result.pos = Vec3(line.c*p + line.s);
        result.pos._w = 1;
    }

    return result;

}

#ifndef NDEBUG
    #include <assert.h>
    namespace geom_tests {

        Plane ABC(  Vec3(4, -3, -2),
                    Vec3(0, 3, -2),
                    Vec3(-4, -3, -2));

        void test_plane(){
            assert(ABC.a == Vec3(0, 0, 48, 96));
            assert(ABC.a.dot4( Vec3(-4, -3, -2)) == 0);
        }
        void test_intersection_line_plane_one(){
            Line a(Vec3(0, 0, 0), Vec3(0, 0, -1));
            LinePlaneIntersectionResult x = intersection(a, ABC);
            assert (x.state == LinePlaneIntersectionResult::State::ONE);
            assert (x.pos == Vec3(0, 0, -2));
        }

        void test_intersection_line_plane_no(){
            Line a(Vec3(1, 2, 3), Vec3(1, 2+1, 3));
            assert (intersection(a, ABC).state == LinePlaneIntersectionResult::State::NO);
        }


        void test_intersection_line_plane_many(){
            Line a(Vec3(0, 0, -2), Vec3(0, 1, -2));
            assert (intersection(a, ABC).state == LinePlaneIntersectionResult::State::MANY);
        }

        int tests(){
            test_intersection_line_plane_one();
            test_intersection_line_plane_no();
            test_intersection_line_plane_many();
            test_plane();
        }
        int test_results = tests();
    }

#endif
