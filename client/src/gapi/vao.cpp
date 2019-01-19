#include "gapi.h"

void VAO::_gen(){
    if (_vao != 0) delete_();
    glGenVertexArrays(1, &_vao);
}

void VAO::delete_(){
#ifndef NDEBUG
    if (_vao == 0){
        throw MyIllegalStateException("Deletion VAO without a call to glGenVertexArrays.");
    }
#endif

    _vbos.clear();
    glDeleteVertexArrays(1, &_vao);
    _vao = 0;
}

VAO &&VAO::data(VBO &&vertex_data, VBO &&color_data, VBO &&texture_data){

    //        if (_vao != 0) delete_();
    //        glGenVertexArrays(1, &_vao);
    if (_vao == 0) _gen();
    _vbos.clear();
    _vbos.push_back(std::move(vertex_data));
    _vbos.push_back(std::move(color_data));
    _vbos.push_back(std::move(texture_data));

    glBindVertexArray(_vao);

    int counter = 0;
    for (auto &vbo : _vbos){
        if (!vbo.isGen()) continue;

        vbo.bind();
        glVertexAttribPointer(counter, vbo._floats_per_vertex, GL_FLOAT, GL_FALSE, vbo._floats_per_vertex * sizeof(float), (void*)0);

        //            typedef void (GLAPIENTRY * PFNGLENABLEVERTEXARRAYATTRIBPROC) (GLuint vaobj, GLuint index);
        glEnableVertexAttribArray(counter);
        counter ++;
    }

    return (VAO&&)*this;
    //        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data, GL_STATIC_DRAW);
}

void VAO::bind(){
    glBindVertexArray(_vao);
}

VAO::~VAO(){
    _delete();
}
