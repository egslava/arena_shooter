#ifndef TEXTURE_H
#define TEXTURE_H

class Texture {
    int _width, _height, _nrChannels;
    unsigned int _gl_id = 0;

    void _gen();

    /** Safe Texture removal. Should be used only internally. A class user should just use destructor. */
    void _delete();
    void delete_();
public:
    Texture(){}
    Texture(const Texture &texture) = delete;
    Texture(Texture &&that);
    bool isGen(){ return _gl_id != 0; }

    Texture&& data(const char *file_name);
    void bind();

    ~Texture();
};

#endif // TEXTURE_H
