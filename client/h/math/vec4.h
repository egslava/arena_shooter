#ifndef VEC4_H
#define VEC4_H

/** the W component is used only for matrix multiplication*/
class Vec3 {
public:
    float _x, _y, _z, _w;
    Vec3(float x=0, float y=0, float z=0, float w=1): _x(x), _y(y), _z(z), _w(w){}

    bool eqXYZ(const Vec3 &another) const {
        // this method is used in tests only, please, use a comparison with Epsilon in other cases
        return  fabs((*this)(0) - another(0) ) < epsilon &&
                fabs((*this)(1) - another(1) ) < epsilon &&
                fabs((*this)(2) - another(2) ) < epsilon;

    }
//    Vec4(

    bool operator == (const Vec3 &another) const {
        // this method is used in tests only, please, use a comparison with Epsilon in other cases
        return  fabs((*this)(0) - another(0) ) < epsilon &&
                fabs((*this)(1) - another(1) ) < epsilon &&
                fabs((*this)(2) - another(2) ) < epsilon &&
                fabs((*this)(3) - another(3) ) < epsilon;

    }
//    Vec4(const Vec4& other) : _x(other._x), _y(other._y), _z(other._z), _w(other._w){}

    inline float operator () (const int i) const EXCEPT_D {
        switch (i) {
        case 0: return _x;
        case 1: return _y;
        case 2: return _z;
        case 3: return _w;

        default:
#ifndef NDEBUG
            throw MyIllegalStateException("Vec4");
#endif
        }
    }

    inline float& operator[] (const int i) EXCEPT_D{
        switch (i) {
        case 0: return _x;
        case 1: return _y;
        case 2: return _z;
        case 3: return _w;
        default:
#ifndef NDEBUG
            throw MyIllegalStateException("Vec4");
#endif
        }
    }

    Vec3& operator += (const Vec3 &other){
        _x += other._x;
        _y += other._y;
        _z += other._z;
//        _w += other._w;
        return *this;
    }


    Vec3& operator -= (const Vec3 &other){
        (*this) += -other;
        return *this;
    }

    Vec3& operator *= (const float scale){
        _x *= scale;
        _y *= scale;
        _z *= scale;
//        _w *= scale;
        return *this;
    }

    Vec3 operator * (const float scale) const {
        Vec3 result(*this);
        result *= scale;
        return result;
    }

    Vec3 operator + (const Vec3 &other) const{
        Vec3 result(*this);
        result += other;
        return result;
    }

    Vec3 operator - () const {
        Vec3 result(*this);
        return result*-1.0;
    }

    Vec3 operator - (const Vec3 &other) const {
        Vec3 result(*this);
        result -= other;
        return result;
    }

    static const Vec3 OX, OY, OZ;
};

inline Vec3 operator * (const float scale, const Vec3 &vec){
    return vec * scale;
}

#endif // VEC4_H
