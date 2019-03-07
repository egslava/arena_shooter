#include "math/math.h"
#include "Config.h"

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

/** Can be negative, depends on a side of the plane*/
float Plane::distance_to(const Vec3 &point) const
{
    return a.dot4(point.xyz(1)) / a.len3();
}

Vec3 Plane::closest(const Vec3 &point) const
{
    return (point - this->a.xyz(0).normed() * this->distance_to(point));
}

Line::Line(const Vec3 &from, const Vec3 &to) {
    this->c = (to-from).xyz(0);
    this->s = from.xyz(1);
}

Vec3 Line::at(float parameter) const {
    return c * parameter + s;
}

float Line::at(const Vec3 &point) const
{
    /*   _    _   _
         y = pc + s. Thus, we can take any of the equations, for instance:
                                        s  -  y
         y  = pc  + s   , thus           x     x
          x     x    x          p=     ----------
                                           c
                                            x
        But we need to ensure that the component makes sense.
    */
    if (fabs(this->c._x) >= epsilon)
        return (point._x - this->s._x) / this->c._x;
    if (fabs(this->c._y) >= epsilon)
        return (point._y - this->s._y) / this->c._y;
    if (fabs(this->c._z) >= epsilon)
        return (point._z - this->s._z) / this->c._z;
}

Segment::Segment(const Vec3 &A, const Vec3 &B)
{
    this->line = Line(A, B);
    this->p1 = this->line.at(A);
    this->p2 = this->line.at(B);
    if (p1 > p2){
        float tmp = p1;
        p1 = p2;
        p2 = tmp;
    }
}

float Segment::closest_p(const Vec3 &point) const
{
    float closest_p_on_line = this->line.at(this->line.closest(point));
    float closest_p_on_segment = fmax(this->p1, fmin(this->p2, closest_p_on_line));  // just clamping
    return closest_p_on_segment;
}

Vec3 Segment::closest(const Vec3 &point) const
{
    return this->line.at(this->closest_p(point));
}

Vec3 Line::closest(const Vec3 &point) const
{
    Vec3 line_dir = this->c.normed();
    Vec3 to_point = (point - this->s);
    Vec3 proj = this->s + line_dir * line_dir.dot3(to_point);
    return proj;
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

        if (tri.in_prism(res.pos)){
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

bool Triangle::in_prism(const Vec3 &point) const
{
    Vec3 n = this->n();
    Plane  BA_plane(this->B, this->A, this->B+n);
    Plane  CB_plane(this->C, this->B, this->C+n);
    Plane  AC_plane(this->A, this->C, this->A+n);
    float d1 = BA_plane.a.dot4((point).xyz(1)),
          d2 = CB_plane.a.dot4((point).xyz(1)),
          d3 = AC_plane.a.dot4((point).xyz(1));
    return d1 > 0 && d2 > 0 && d3 > 0;
}

Vec3 Triangle::closest(const Vec3 &point) const
{
    Vec3 closest_to_plane = Plane(this->A, this->B, this->C).closest(point);
    if (in_prism(closest_to_plane)){
        return closest_to_plane;
    }
    Segment a(this->A, this->B);
    Segment b(this->B, this->C);
    Segment c(this->C, this->A);
    Vec3 p1 = a.closest(point);
    Vec3 p2 = b.closest(point);
    Vec3 p3 = c.closest(point);
    float d1 = (p1 - point).len3();
    float d2 = (p2 - point).len3();
    float d3 = (p3 - point).len3();

    if (d1 <= d2 && d1 <= d3){
        return p1;
    } else if (d2 <= d1 && d2 <= d3){
        return p2;
    } else {
        return p3;
    }
//    sphere_player.C = pos;

//    Line l (pos, pos+tri.n());
//    LinePlaneIntersectionResult p=intersection(l, tri);
//    if (p.state != p.State::ONE && !in (sphere_player, tri.A) && !in (sphere_player, tri.B) && !in (sphere_player, tri.C))
//        continue;

//    float d = static_cast<Plane>(tri).distance_to(pos);
//    float d2 = static_cast<Plane>(tri).distance_to(pos);
//    // printf("line: c: %f;%f;%f, s: %f;%f;%f\n", l.c._x, l.c._y, l.c._z, l.s._x, l.s._y, l.s._z);
//    // printf("intersection. x: %f, y: %f, z: %f\n", p.pos._x, p.pos._y, p.pos._z);
//    // printf("triangle. %0.2f,%0.2f,%0.2f;  %0.2f,%0.2f,%0.2f;  %0.2f,%0.2f,%0.2f\n", tri.A._x, tri.A._y, tri.A._z, tri.B._x, tri.B._y, tri.B._z, tri.C._x, tri.C._y, tri.C._z);
//    if (fabs(d) <= min_distance && p.state == p.State::ONE){ //&& fabs(d) <= nearest_distance){
//        float nearest_distance = fabs(d);
//        //                        res = tri.n() * dir.len3();
//        //                        res = tri.n() * (tri.n().dot3(dir));
//        //                        res += tri.n() * (0.15f-fabs(d)) * (d<0?-1:1);
//        cres += (d<0?-1:1)*tri.n() * (min_distance-fabs(d));
//        n_collisions ++;
//    }
}

bool in(const Ball &sphere, const Vec3 &point){
    return (point - sphere.C).len3() < sphere.R;
}

bool in(const Frustum &frustum, const Vec3 &point){
    /* */
    float center_in = 0;
    for (const Plane &plane : frustum.planes){
        // counting only negative values
        center_in += fmin(0, point.dot3(plane.normal()));
    }

    return fabs(center_in) < epsilon;  // shouldn't count any negative value
}

bool in(const Frustum &frustum, const Ball &sphere){
    /*
     * Sphere in frustum:
     *
     * # if center in frustum
     * center_in = sum(max(0, dot(sphere.pos, plane.normal)) for plane in frustum ) > 0
     * if (center_in): return true
     *
     * # otherwise a bit more complicated logic
     * # First of all, we check: does the sphere intersect any plane of frustum
     * sphere_touches_plane = sum(max(0, distance(sphere.pos, plane) + r) for plane in frustum ) > 0
     *
     * But sometimes, it can give false-positives, like this one:
     * docs-srcs/source/res/bspheres-frustum-culling-bad-case.png
     *
     * Thus, we need an additional check: we're going to check, whether the View Frustum is
     * inside of the sphere:
     * for point in frustum:
     *     if distance(point, sphere.pos) < sphere.r:
     *         return true
     * return false
     */

    // 1. obviously
    if (in(frustum, sphere.C)){
        return true;
    }

    // 2. docs-srcs/source/res/bspheres-frustum-culling-bad-case.png
    // Thanks for the inspiration:
    // http://www.iquilezles.org/www/articles/frustumcorrect/frustumcorrect.htm
    for (const Plane &plane : frustum.planes){
        if (plane.distance_to(sphere.C) < -sphere.R){
            return false;
        }
    }

    for (const Vec3& frustum_point : frustum.points){
        if (in(sphere, frustum_point)){
            return true;
        }
    }
    return false;
}

void pull_away(const Triangle &tri, float min_distance, Vec3 &pos, bool &collisions_found, bool &on_ground){
    Vec3 collision_point = tri.closest(pos);
    Vec3 col_dir = collision_point - pos;
    Vec3 normal = tri.n();
    if (isnan(normal._x) || isnan(normal._y) || isnan(normal._z)){
        collisions_found = false;
        return;
    }

    float d = col_dir.len3();
    if (d >= min_distance){
        collisions_found = false;
        return;
    }

    // backface culling
    if (col_dir.dot3(normal) > 0){
        collisions_found = false;
        return;
    }

    collisions_found = true;

    if (fabsf(normal._y) > fabsf(normal._x) && fabsf(normal._y) > fabsf(normal._z)) {
        on_ground = true;
    }
    // printf("collision: %0.3f, %0.3f, %0.3f.  pos: %0.3f, %0.3f, %0.3f\n",
    //       collision_point._x, collision_point._y, collision_point._z,
    //       pos._x, pos._y, pos._z);
    // printf("triangle. %0.3f,%0.3f,%0.3f;  %0.3f,%0.3f,%0.3f;  %0.3f,%0.3f,%0.3f\n", tri.A._x, tri.A._y, tri.A._z, tri.B._x, tri.B._y, tri.B._z, tri.C._x, tri.C._y, tri.C._z);
    fflush(stdout);
    // printf("intersection. x: %f, y: %f, z: %f\n", p.pos._x, p.pos._y, p.pos._z);

    Vec3 correction = 1*(d<0?-1:1)*normal * (min_distance-fabs(d)); // * fabs((collision_point - pos).dot3(tri.n()));

    if (isnan(correction._x) || isnan(correction._y) || isnan(correction._z) ){
        throw MyIllegalStateException("NaN!");
    };
    pos += correction;
}

Vec3 pull_away(const std::vector<Triangle> &mesh, Vec3 pos, float min_distance, bool &collisions_found, bool &on_ground)
{
    constexpr int max_passes = 10;
//    int n_collisions = 0;
    collisions_found = false;
    for (int i_pass = 0; i_pass < max_passes; i_pass++){
        bool still_collisions = false;
        for (const Triangle &tri: mesh){
            pull_away(tri, min_distance, pos, still_collisions, on_ground);

            if (still_collisions) {
                collisions_found = true;
                continue;
            }
        }

        if (!still_collisions)break;
    }

    return pos;
}

#include <limits>
void AABB::set(const std::vector<Triangle> &mesh) {
    float min_x, max_x, min_y, max_y, min_z, max_z;
    min_x = min_y = min_z = std::numeric_limits<decltype(min_x)>::max();
    max_x = max_y = max_z = std::numeric_limits<decltype(max_x)>::min();

    for (int i = 0; i < mesh.size(); ++i) {
        const Triangle &tri = mesh[i];
        min_x = fmin(tri.A._x, min_x);
        min_y = fmin(tri.A._y, min_y);
        min_z = fmin(tri.A._z, min_z);
        min_x = fmin(tri.B._x, min_x);
        min_y = fmin(tri.B._y, min_y);
        min_z = fmin(tri.B._z, min_z);
        min_x = fmin(tri.C._x, min_x);
        min_y = fmin(tri.C._y, min_y);
        min_z = fmin(tri.C._z, min_z);

        max_x = fmax(tri.A._x, max_x);
        max_y = fmax(tri.A._y, max_y);
        max_z = fmax(tri.A._z, max_z);
        max_x = fmax(tri.B._x, max_x);
        max_y = fmax(tri.B._y, max_y);
        max_z = fmax(tri.B._z, max_z);
        max_x = fmax(tri.C._x, max_x);
        max_y = fmax(tri.C._y, max_y);
        max_z = fmax(tri.C._z, max_z);
    }

    this->set(min_x, max_x, min_y, max_y, min_z, max_z);
}

void AABB::set(const Ball &ball) {
    float min_x = ball.C._x - ball.R;
    float max_x = ball.C._x + ball.R;
    float min_y = ball.C._y - ball.R;
    float max_y = ball.C._y + ball.R;
    float min_z = ball.C._z - ball.R;
    float max_z = ball.C._z + ball.R;

    this->set(min_x, max_x, min_y, max_y, min_z, max_z);
}

void AABB::set(const float min_x, const float max_x, const float min_y, const float max_y, const float min_z, const float max_z)
{
//    this->C._x = (max_x - min_x) / 2;
//    this->C._y = (max_y - min_y) / 2;
//    this->C._z = (max_z - min_z) / 2;

//    this->R = Vec3(max_x, max_y, max_z) - this->C;

    this->R._x = (max_x - min_x) / 2;
    this->R._y = (max_y - min_y) / 2;
    this->R._z = (max_z - min_z) / 2;

    this->C = Vec3(min_x, min_y, min_z) + this->R;
}

/** Do 2 AABB intersect each other? */
bool in(const AABB &aabb1, const AABB &aabb2) {
    Vec3 min_distance = aabb1.R + aabb2.R;
    Vec3 distance = (aabb2.C - aabb1.C).abs();

    Vec3 inter = distance - min_distance;
    return inter._x < 0 && inter._y < 0 && inter._z < 0;
}

#ifdef RUN_TESTS
    #include <assert.h>
    #include "gapi/gapi.h"
    #include "gapi/loaders/mymodel.h"
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

        Frustum frustum = Frustum().init(
            Vec3(-5.72334, -3.34758, -3.34758), Vec3(-1, -1, -1), Vec3(-5.72334, -3.34758, 3.34758), Vec3(1, -1, 1),
            Vec3(-5.72334, 3.34758, -3.34758), Vec3(1, 1, -1), Vec3(-5.72334, 3.34758, 3.34758), Vec3(1, 1, 1)
        );

        void test_sphere_not_in_frustum(){
            /* I've modelled this situation in Blender first. You can check it here:
            docs-src/source/res/code-docs/tests/sphere-not-in-frustum.blend */
            Ball sphere;
            sphere.C = Vec3(-11.1052, -10.7348, 5.54151);
            sphere.R = 6.90636;

            assert (false == in(frustum, sphere));
        }

        void test_sphere_in_frustum(){
            /* I've modelled this situation in Blender first. You can check it here:
            docs-src/source/res/code-docs/tests/sphere-in-frustum.blend */
            Ball sphere;
            sphere.C = Vec3(-9.1052, -8.7348, 5.54151);
            sphere.R = 6.90636;

            assert (in(frustum, sphere));
        }

        void test_plane_closest(){
            Plane plane (Vec3(0,0,0), Vec3(0, 2, 0), Vec3(2, 0, 0));
            assert (plane.closest(Vec3(2, 2, 0)).eqXYZ(Vec3(2, 2)));
            assert (plane.closest(Vec3(-1, -1, 0)).eqXYZ(Vec3(-1, -1)));
            assert (plane.closest(Vec3(-1, -1, 2)).eqXYZ(Vec3(-1, -1)));
            assert (plane.closest(Vec3(-1, -1, -3)).eqXYZ(Vec3(-1, -1)));

            assert (plane.closest(Vec3(0.5, 0.5, 0)).eqXYZ( Vec3(0.5, 0.5, 0)));
            Vec3 res = plane.closest(Vec3(0.5, 0.5, 2));
            assert (res.eqXYZ( Vec3(0.5, 0.5, 0)));
            assert (plane.closest(Vec3(0.5, 0.5, -3)).eqXYZ(Vec3(0.5, 0.5, 0)));
        }

        void test_segment_closest(){
            Vec3 A(0,0), B(0, 2), C(2, 0);
            Segment a(A, B), b(B, C), c(C, A);
            Vec3 p(2, 2);
            assert(a.closest(p).eqXYZ(Vec3(0,2)));
            assert(b.closest(p).eqXYZ(Vec3(1,1)));
            assert(c.closest(p).eqXYZ(Vec3(2,0)));

            Vec3 p2(2,2,3);
            assert(a.closest(p2).eqXYZ(Vec3(0,2)));
            assert(b.closest(p2).eqXYZ(Vec3(1,1)));
            assert(c.closest(p2).eqXYZ(Vec3(2,0)));

            Vec3 p3(2,2,-5);
            assert(a.closest(p3).eqXYZ(Vec3(0,2)));
            assert(b.closest(p3).eqXYZ(Vec3(1,1)));
            assert(c.closest(p3).eqXYZ(Vec3(2,0)));
        }


        void test_triangle_closest(){
            Triangle tri (Vec3(0,0,0), Vec3(0, 2, 0), Vec3(2, 0, 0));
            assert (tri.closest(Vec3(2, 2, 0)).eqXYZ(Vec3(1, 1, 0)));
            assert (tri.closest(Vec3(-1, -1, 0)).eqXYZ(Vec3(0, 0, 0)));
            assert (tri.closest(Vec3(-1, -1, 2)).eqXYZ(Vec3(0, 0, 0)));
            assert (tri.closest(Vec3(-1, -1, -3)).eqXYZ(Vec3(0, 0, 0)));

            assert (tri.closest(Vec3(0.5, 0.5, 0)).eqXYZ( Vec3(0.5, 0.5, 0)));
            Vec3 res = tri.closest(Vec3(0.5, 0.5, 2));
            assert (res.eqXYZ( Vec3(0.5, 0.5, 0)));
            assert (tri.closest(Vec3(0.5, 0.5, -3)).eqXYZ(Vec3(0.5, 0.5, 0)));
//            просто создать тестовый треугольник и чекнуть, что возвращаются
//            корректные результаты (сейчас это не так, т.к. часто вылезают NaN'ы)
        }

        bool on_ground_stub, on_collision_found_stub;
        void test_pull_away_tri(){

            Triangle tri(Vec3(0,0), Vec3(2,0), Vec3(0,2));
            bool collisions_found;
            Vec3 pos(0,0,0);
            pull_away(tri, 1.0f, pos, collisions_found, on_ground_stub);
            assert(collisions_found);
            assert(pos == Vec3(0,0,1));

            // regression tests
            Triangle tri2(Vec3(-24.875,2.000,24.875), Vec3(-24.875,2.000,-24.875), Vec3(-24.875,2.000,24.875));
            Vec3 pos2(-24.878, 1.042, 19.562);
            pull_away(tri2, 1.1f, pos2, collisions_found, on_ground_stub);
            assert(!isnan(pos2._x) && !isnan(pos2._y) && !isnan(pos2._z) );

//            assert
            //            collision: -24.875;2.000;19.562.

            //collision: -24.875;2.000;19.305.  pos: -23.842;2.154;19.305
            //triangle. -24.875,2.000,24.875;  -24.875,2.000,-24.875;  -24.875,2.000,24.875

            //collision: 24.875;2.000;23.775.  pos: 23.813;1.732;23.775
            //triangle. 24.875,2.000,-24.875;  24.875,2.000,24.875;  24.875,2.000,-24.875

            //collision: 24.875;2.000;23.775.  pos: 23.883;1.534;23.775
            //triangle. 24.875,2.000,-24.875;  24.875,2.000,24.875;  24.875,2.000,-24.875
        }

        void test_move_collidings(){
            // regression test. Captured from logs
            // collision: -24.849;2.000;24.875.  pos: -24.849;0.904;24.808



            Model cube2x;
            MyModel::VBOs vbos = MyModel::load("./res/tests/cube2x.model");
            cube2x._fill_triangles(vbos);
            MyModel::free(vbos);

            Vec3 res = pull_away(cube2x._triangles, Vec3(0.9, 0.9, 0.9), 2.0, on_collision_found_stub, on_ground_stub);
            assert(Vec3(0,0,0).eqXYZ(res));


//            cube.load("./res/tests/cube2x.model", Texture());


//            Triangle tri( Vec3(24.875,2.000,24.875), Vec3(-24.875,2.000,24.875), Vec3(24.875,2.000,24.875) );
//            Vec3 pos(-24.849, 0.904, 24.808 );


//            Vec3 collision_point = tri.closest(pos);
//            Vec3 col_dir = collision_point - pos;
//            Vec3 normal = tri.n();

//            float min_distance = 1.1;
//            float d = col_dir.len3();
//            assert (d < min_distance);
//            assert (col_dir.dot3(normal) <= 0);  // backface culling

//            if (fabs(d) <= min_distance){ // && p.state == p.State::ONE){ //&& fabs(d) <= nearest_distance){
//                float nearest_distance = fabs(d);
//                //                        res = tri.n() * dir.len3();
//                //                        res = tri.n() * (tri.n().dot3(dir));
//                //                        res += tri.n() * (0.15f-fabs(d)) * (d<0?-1:1);

////                     cres += 1*(d<0?-1:1)*normal * (min_distance-fabs(d)); // * fabs((collision_point - pos).dot3(tri.n()));
//                pos += 1*(d<0?-1:1)*normal * (min_distance-fabs(d)); // * fabs((collision_point - pos).dot3(tri.n()));
//            }

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
            test_sphere_not_in_frustum();
            test_sphere_in_frustum();
            test_plane_closest();
            test_segment_closest();
            test_triangle_closest();
            test_pull_away_tri();
            test_move_collidings();
        }
        int test_results = tests();
    }

#endif
