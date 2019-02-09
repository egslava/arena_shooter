#include "scene/model.h"
#include "gapi/loaders/mymodel.h"

Model&& Model::load(const char *filename, Texture &&tex) {
    MyModel::VBOs vbos = MyModel::load(filename);

    VAO&& result = this->_vao.data(VBO().data(vbos.pos), VBO().data(vbos.nor), VBO().data(vbos.tex, 2));

    this->_triangles.reserve(vbos.pos.size() / 9);

    for (int i = 0; i < vbos.pos.size(); i += 9){
        this->_triangles.emplace_back(
                                      Vec3(vbos.pos[i+0], vbos.pos[i+1], vbos.pos[i+2], 1),
                                      Vec3(vbos.pos[i+3], vbos.pos[i+4], vbos.pos[i+5], 1),
                                      Vec3(vbos.pos[i+6], vbos.pos[i+7], vbos.pos[i+8], 1));
    }
    MyModel::free(vbos);

    this->_tex = std::move(tex);
    return (Model&&)*this;
}

void Model::draw()
{
    this->_tex.bind();
    this->_vao.bind();
    glDrawArrays(GL_TRIANGLES, 0, this->_triangles.size() * 3);
}
