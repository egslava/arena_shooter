#ifndef MATRIX_H
#define MATRIX_H

class Mat4x4 {
    Mat4x4(){};
public:
    friend Mat4x4 operator * (const Mat4x4 &mat1, const Mat4x4& mat2) noexcept;
    friend Vec3 operator * (const Mat4x4 &mat1, const Vec3& vec4) noexcept;
    Mat4x4& operator = (const Mat4x4& mat2) noexcept;

    static constexpr int ROWS = 4, COLS = 4;

    Mat4x4(const float data[ROWS*COLS]) {
        memcpy(this->_data, data, sizeof(float) * ROWS * COLS);
    }
    Mat4x4(const Mat4x4 &mat4x4): Mat4x4(mat4x4._data){}


    float _data[ROWS * COLS];

    inline float operator () (const int i, const int j) const noexcept {
        return _data[i*COLS + j];
    }

    inline float& at(const int i, const int j) noexcept{
        return _data[i*COLS + j];
    }

    inline bool operator == (const Mat4x4 &another) const;
    inline Vec3 row(int i) const{ return Vec3(_data[i*COLS], _data[i*COLS+1], _data[i*COLS+2], _data[i*COLS + 3]); }
    inline Vec3 col(int j) const{ return Vec3(_data[0+j], _data[1+j], _data[2+j], _data[3+j]); }
    Vec3 inline oX() const{ return col(0); }
    Vec3 inline oY() const{ return col(1); }
    Vec3 inline oZ() const{ return col(2); }

    static Mat4x4 set_rot_x(float angle);
    static Mat4x4 set_rot_y(float angle);
    static Mat4x4 set_rot_z(float angle);
    static Mat4x4 set_scale(const Vec3 &vec);
    static Mat4x4 set_scale(float sx, float sy, float sz);
    Mat4x4 T() const noexcept;
    static Mat4x4 set_translation(float tx, float ty, float tz);
    static Mat4x4 set_translation(const Vec3 &vec3);
    static Mat4x4 set_ortho(float left, float right, float bottom, float top, float nearVal, float farVal);
    static Mat4x4 set_perspective(float fovy, float aspect, float zNear, float zFar);

    static Mat4x4 rand(float min = -1.0f, float max=1.0);

//    template <typename Number>
//    Mat4x4& operator *= (const Number n) noexcept{
//        for(int i = 0; i < ROWS*COLS; i++){
//            _data[i] *= n;
//        }
//        return *this;
//    }
    static const Mat4x4 I;
    static const Mat4x4 O;
};


Mat4x4 operator * (const Mat4x4 &mat1, const Mat4x4& mat2) noexcept;

#endif // MATRIX_H
