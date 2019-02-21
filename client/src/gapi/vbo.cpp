#include "gapi/gapi.h"

void VBO::_gen(){
    if (_vbo != 0) delete_();
    fprintf(stdout, "glGenBuffers()\n");
    fflush(stdout);
    glGenBuffers(1, &_vbo);
}

void VBO::_delete(){
    if(_vbo != 0) delete_();
}

void VBO::delete_(){
    #ifndef NDEBUG
    if (_vbo == 0){
        throw MyIllegalStateException("An attempt to delete VBO without Gen it first has been detected.");
    }
    #endif

    glDeleteBuffers(1, &_vbo);
    fprintf(stdout, "glDeleteBuffers()\n");
    fflush(stdout);
    _vbo = 0;
}

VBO::VBO(VBO &&that){
    printf("move constructor\n");
    _vbo = that._vbo;
    that._vbo = 0;
    _floats_per_vertex = that._floats_per_vertex;
}


VBO &&VBO::data(const std::vector<float> data, unsigned int floats_per_vertex){
    if (_vbo == 0) _gen();

    this->_floats_per_vertex = floats_per_vertex;

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);

    return (VBO&&)*this;
}

void VBO::bind(){
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
}

VBO::~VBO(){
    _delete();
}
