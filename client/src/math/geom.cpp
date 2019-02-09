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

float Plane::distance_to(const Vec3 &point) const
{
    return a.dot4(point.xyz(1)) / a.len3();
}

Line::Line(const Vec3 &from, const Vec3 &to) {
    this->c = (to-from).xyz(0);
    this->s = from.xyz(1);
}

Vec3 Line::at(float parameter) const {
    return c * parameter + s;
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
        result.pos = line.at(p);
        result.pos._w = 1;
    }

    return result;
}

LinePlaneIntersectionResult intersection(const Line &line, const Triangle &tri) {
    LinePlaneIntersectionResult res = intersection(line, static_cast<Plane>(tri));
    switch (res.state) {
    case res.State::MANY:
//        throw MyNotYetImplementedException("intersection line-triangle MANY");
        // TODO:
//        printf("State::MANY\n");
    case res.State::NO:
        return res;
    case res.State::ONE: {

        Vec3 n = tri.n();
        Plane  BA_plane(tri.B, tri.A, tri.B+n);
        Plane  CB_plane(tri.C, tri.B, tri.C+n);
        Plane  AC_plane(tri.A, tri.C, tri.A+n);
        float d1 = BA_plane.a.dot4((res.pos).xyz(1)),
              d2 = CB_plane.a.dot4((res.pos).xyz(1)),
              d3 = AC_plane.a.dot4((res.pos).xyz(1));
//        float d1 = (res.pos - tri.A).dot3 (tri.B-tri.A),
//              d2 = (res.pos - tri.B).dot3 (tri.C-tri.B),
//              d3 = (res.pos - tri.C).dot3 (tri.A-tri.C);
        if (
                (d1 > 0 &&
             d2 > 0 &&
             d3 > 0)
//             ||
//                (d1 < 0 &&
//               d2 < 0 &&
//               d3 < 0)
             ){
            return res;
        }
        res.state = res.State::NO;
        return res;
    }
#ifndef NDEBUG
    default:
        throw MyIllegalStateException("Invalid LinePlaneIntersectionResult.state");
        break;
#endif
    }
}


LineLineIntersectionResult intersection(const Line &line1, const Line &line2) {
    // I started to do this method, but, I'm not sure it's really needed.
    //
    throw MyNotYetImplementedException("!");
//    LinePlaneIntersectionResult p = intersection(line, (Plane)tri);
    float sx = (line1.s._x - line2.s._x);
    float sy = (line1.s._y - line2.s._y);
    float sz = (line1.s._z - line2.s._z);
    float dividendZ = line2.c._x * sy - line2.c._y * sx;
    float divisorX  = line1.c._y * line2.c._z - line1.c._z * line2.c._y;
    float divisorY  = line1.c._x * line2.c._z - line1.c._z * line2.c._x;
    float divisorZ  = line1.c._x * line2.c._y - line1.c._y * line2.c._x;

    LineLineIntersectionResult res;
    if (fabs(divisorZ) <= epsilon ){
        if (fabs(dividendZ) <= epsilon){
            res.state = res.State::MANY;
        } else {
            res.state = res.State::NO;
        }
    } else {
        res.state = res.State::ONE;
        res.p1 = dividendZ / divisorZ;
        res.p2 = (line1.c._x * sy - line1.c._y * sx) / divisorZ;
    }

    return res;
}


Vec3 Triangle::n() const{
    return (B-A).cross3(C-B).normed();
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


        void test_intersection_line_line_no(){
            Line line1(Vec3(1,1,1), Vec3(1, 2, 1));
            Line line2(Vec3(2,1,1), Vec3(2, 2, 1));
            LineLineIntersectionResult res = intersection(line1, line2);
            assert(res.state == res.State::NO);
        }

        void test_intersection_line_line_many(){
            Line line1(Vec3(2,2,2), Vec3(-2, -2, -2));
            Line line2(Vec3(-2,-2,-2), Vec3(2, 2, 2));
            assert(intersection(line1, line2).state == LineLineIntersectionResult::State::MANY);

            Line line3(Vec3( 0, 0, 0), Vec3( 2, 0, 0));
            Line line4(Vec3( 2, 0, 0), Vec3(-4, 0, 0));
            assert(intersection(line3, line4).state == LineLineIntersectionResult::State::MANY);

            Line line5(Vec3( 0, 0, 0), Vec3(0,  2, 0));
            Line line6(Vec3( 0, 2, 0), Vec3(0, -4, 0));
            assert(intersection(line5, line6).state == LineLineIntersectionResult::State::MANY);

            Line line7(Vec3( 0, 0, 0), Vec3(0, 0,  2));
            Line line8(Vec3( 0, 0, 2), Vec3(0, 0, -4));
            assert(intersection(line7, line8).state == LineLineIntersectionResult::State::MANY);
        }

        void test_intersection_line_line_one(){
            Line line1(Vec3(2,2,2), Vec3(2, 3, 2));  // OY
            Line line2(Vec3(2,2,2), Vec3(3, 2, 2));  // OX
            LineLineIntersectionResult res = intersection(line1, line2);
            assert(res.state == res.State::ONE);
            assert(line1.at(res.p1) == line2.at(res.p2));
            assert(line1.at(res.p1).eqXYZ(Vec3(2, 2, 2)));

            Line line3(Vec3(0,0,0), Vec3(1, 0, 0));  // OX
            Line line4(Vec3(0,0,0), Vec3(0, 0, -1));  // OZ
            LineLineIntersectionResult res2 = intersection(line3, line4);
            assert(res2.state == res2.State::ONE);
            assert(line3.at(res2.p1) == line4.at(res2.p2));
            assert(line4.at(res2.p1).eqXYZ(Vec3(0, 0, 0)));
        }


        void test_intersection_line_triangle_no(){
            Line line1(Vec3(0,0, 0), Vec3(0, 0, 2));
            Triangle tri1(Vec3(0,0,0), Vec3(0,1,0), Vec3(1,0,0));
            assert (intersection(line1, tri1).state == LinePlaneIntersectionResult::State::NO);

            Line line2(Vec3(3,3, 3), Vec3(3, 4, 3));
            Triangle tri2(Vec3(0,1,0), Vec3(1,1,0), Vec3(0,1,1));
            assert (intersection(line2, tri2).state == LinePlaneIntersectionResult::State::NO);

            Line line3(Vec3(3,3, 3), Vec3(2, 3, 3));
            Triangle tri3(Vec3(1,-1,-1), Vec3(1,1,1), Vec3(1,-1,1));
            assert (intersection(line3, tri3).state == LinePlaneIntersectionResult::State::NO);


            Line line4(Vec3(2,2, 2), Vec3(1, 2, 2));
            Triangle tri4(Vec3(2, -2, 2), Vec3(2, 5,3), Vec3(2, 0,3));
            LinePlaneIntersectionResult res = intersection(line4, tri4);
            assert (res.state == LinePlaneIntersectionResult::State::NO);
//            assert (res.pos == Vec3(2,2,2));

            // regression test. Captured with logs:
            // intersection. x: 0.047832, y: 1.322341, z: -0.000000
            // triangle. 0.9,-0.5,0.0;0.9,0.5,0.0;-0.9,0.5,0.0
            // So, as it's shown here, the intersection is outside of the triangle
//            Triangle tri5(Vec3(0.9,-0.5,0.0), Vec3(0.9,0.5,0.0), Vec3(-0.9,0.5,0.0));
//            Line line5(Vec3(0,0,0), Vec3(0,0,0));
//            line5.c = Vec3(0.000735, 0.008059, -0.009174, 0);
//            line5.s = Vec3(0.046629, 1.309164, 0.015000, 1);

            Triangle tri5(Vec3(-0.45,-0.14,0.30), Vec3(-0.45,-0.29,-0.21), Vec3( -0.45,-0.29,0.30));
            Line line5(Vec3(0,0,0), Vec3(0,0,0));
            line5.c = Vec3(0.012180, 0.000000, -0.003068);
            line5.s = Vec3(0.395989, -0.493816, 0.122126, 1);
//            intersection. x: -0.449781, y: -0.493816, z: 0.335192
            LinePlaneIntersectionResult res5 = intersection(line5, tri5);
            assert (res5.state == LinePlaneIntersectionResult::State::NO);

        }


        void test_intersection_line_triangle_one(){
            Line line1(Vec3(2,3, 4), Vec3(2, 3, 5));
            Triangle tri1(Vec3(-10,-10, -2), Vec3(-2,10,-2), Vec3(10,-10,-2));
            assert (intersection(line1, tri1).state == LinePlaneIntersectionResult::State::ONE);
            assert (intersection(line1, tri1).pos == Vec3(2,3,-2));

            Line line2(Vec3(1,2, 1), Vec3(1, 3, 1));
            Triangle tri2(Vec3(-2, 2, 2), Vec3(2,2,-3), Vec3(2, 2,2));
            assert (intersection(line2, tri2).state == LinePlaneIntersectionResult::State::ONE);
            assert (intersection(line2, tri2).pos == Vec3(1,2,1));

//            Line line3(Vec3(2,2, 2), Vec3(1, 2, 2));
//            Triangle tri3(Vec3(2, -2, 2), Vec3(2, 5,3), Vec3(2, 0,3));
//            LinePlaneIntersectionResult res = intersection(line3, tri3);
//            assert (res.state == LinePlaneIntersectionResult::State::ONE);
//            assert (res.pos == Vec3(2,2,2));

//            Line line2(Vec3(3,3, 3), Vec3(3, 4, 3));
//            Triangle tri2(Vec3(0,1,0), Vec3(1,1,0), Vec3(0,1,1));
//            assert (intersection(line2, tri2).state == LinePlaneIntersectionResult::State::NO);

//            Line line3(Vec3(3,3, 3), Vec3(2, 3, 3));
//            Triangle tri3(Vec3(1,-1,-1), Vec3(1,1,1), Vec3(1,-1,1));
//            assert (intersection(line3, tri3).state == LinePlaneIntersectionResult::State::NO);
        }

        void test_triangle_normal_is_correct(){
            Triangle t(Vec3(-2, -2, -2),
                       Vec3( 2, -2, -2),
                       Vec3( 1,  2, -2));
            assert (t.n() == Vec3(0, 0, 1));
        }

        void test_distance_plane_point(){
            Plane p(Vec3(0, 0, 0), Vec3(0, 1, 0), Vec3(1, 1, 0));
            assert( fabs(p.distance_to(Vec3(0, 0, -1)) - 1) <= epsilon );
            assert( fabs(p.distance_to(Vec3(0, 0,  1)) + 1) <= epsilon );
        }

        int tests(){
            test_plane();
            test_intersection_line_plane_one();
            test_intersection_line_plane_no();
            test_intersection_line_plane_many();
            test_intersection_line_triangle_no();
            test_intersection_line_triangle_one();
//            test_intersection_line_line_no();
//            test_intersection_line_line_many();
//            test_intersection_line_line_one();
            test_triangle_normal_is_correct();
            test_distance_plane_point();
        }
        int test_results = tests();
    }

#endif
