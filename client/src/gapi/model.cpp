#include "gapi/gapi.h"

Model &&Model::color(const Vec3 color)
{
    this->_color = color;
    return static_cast<Model&&>(*this);
}

Model&& Model::load(const char *filename, Texture &&tex) {
    MyModel::VBOs vbos = MyModel::load(filename);

    std::vector<VBO> gpu_vbos;
    gpu_vbos.push_back( VBO().data(vbos.pos) );
    gpu_vbos.push_back( VBO().data(vbos.nor) );

	if (vbos.tex.size() == 0) vbos.tex.resize(vbos.pos.size());

    gpu_vbos.push_back( VBO().data(vbos.tex, 2) );

    VAO&& result = this->_vao.data(std::move(gpu_vbos));

    this->_fill_triangles(vbos);
    MyModel::free(vbos);

    this->_tex = std::move(tex);
    return (Model&&)*this;
}

void Model::_fill_triangles(const MyModel::VBOs &vbos)
{
    this->_triangles.resize(0);
    this->_triangles.reserve(vbos.pos.size() / 9);

    for (int i = 0; i < vbos.pos.size(); i += 9){
        this->_triangles.emplace_back(
                                      Vec3(vbos.pos[i+0], vbos.pos[i+1], vbos.pos[i+2], 1),
                                      Vec3(vbos.pos[i+3], vbos.pos[i+4], vbos.pos[i+5], 1),
                                      Vec3(vbos.pos[i+6], vbos.pos[i+7], vbos.pos[i+8], 1));
    }
}

void Model::draw()
{
	if (this->_triangles.size() > 0) {  // VAO isn't initialized, thus, shoudln't be used
		this->_tex.bind();
		this->_vao.bind();
		glDrawArrays(GL_TRIANGLES, 0, this->_triangles.size() * 3);
	}
}
