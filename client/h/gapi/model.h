#ifndef MODEL_H
#define MODEL_H

#include "gapi/loaders/mymodel.h"
#include "math/geom.h"

class Model {
    VAO _vao;
    Texture _tex;
public:
    Vec3 _color = Vec3(1.0, 1.0, 1.0).bright_rgb(1.0f);
    std::vector<Triangle> _triangles;  // for collisions and num of triangles

    Model(){}
    Model(const Model&) = delete;
    Model(Model &&that) = default;
    Model &&operator =(Model &&that) {
        this->_vao = std::move(that._vao);
        this->_tex = std::move(that._tex);
        this->_color = that._color;
        this->_triangles = std::move(that._triangles);
    }
//    {
//        this->_gl_id = that._gl_id;
//        this->_width = that._width;
//        this->_height = that._height;
//        this->_nrChannels = that._nrChannels;
//        that._gl_id = 0;
//        return (Texture&&)*this;
//    }

//    Model(VAO &&vao): _vao(std::move(vao)) {}

    Model&& color(const Vec3 color);
    Model&& load(const char *filename, Texture &&Texture);
    void _fill_triangles(const MyModel::VBOs &vbos);
    void draw();
};

#endif // MODEL_H
