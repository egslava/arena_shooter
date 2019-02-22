#pragma once

class VAO {
    friend class Particles;
    GLuint _vao = 0;
    std::vector<VBO> _vbos;

    void _gen();

    void delete_();

    /** Safe VAO removal. Should be used only internally. A class user should just use destructor. */
    void _delete(){
        if(_vao != 0) delete_();
    }

public:
    VAO(){}
    VAO(const VAO &that)=delete;
    VAO(VAO &&that){
        _vao = that._vao;
        _vbos = std::move(that._vbos);
        that._vao = 0;
    }


    VAO&& data(VBO &&vertex_data, VBO &&normal_data, VBO &&texture_data);
    void bind();

    ~VAO();
};
