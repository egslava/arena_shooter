#pragma once


class VBO {
    friend class VAO;

    unsigned int _vbo = 0;
    unsigned int _floats_per_vertex;
    void _gen();

    /** Safe VBO removal. Should be used only internally. A class user should just use destructor. */
    void _delete();
    void delete_();
public:
    VBO(){}
    VBO(const VBO &vbo) = delete;
    VBO(VBO &&that);
    VBO&& data(const std::vector<float> data, unsigned int floats_per_vertex=3);
    bool isGen(){ return _vbo != 0; }
    void bind();
    ~VBO();
};
