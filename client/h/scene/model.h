#ifndef MODEL_H
#define MODEL_H

#include "gapi.h"
#include "scene/camera.h"
#include "math/geom.h"

class Model {
    VAO _vao;
    Texture _tex;
public:
    std::vector<Triangle> _triangles;  // for collisions and num of triangles
    Camera node;

    Model(){}
    Model(const Model&) = delete;
    Model(Model &&that) = default;
//    Model(VAO &&vao): _vao(std::move(vao)) {}

    Model&& load(const char *filename);
    void draw();
};

#endif // MODEL_H
