//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"

#include "gapi/loaders/pvr.h"

#include "gapi.h"


void Texture::_gen(){
    if (_gl_id != 0) delete_();
    fprintf(stdout, "glGenTextures()\n");
    fflush(stdout);

    glGenTextures(1, &_gl_id);

    #ifndef NDEBUG
        if (_gl_id == 0){
            throw MyGlException("Can not glGenTexture()");
        }
    #endif
}

void Texture::_delete(){
    if(_gl_id != 0) delete_();
}

void Texture::delete_(){
    #ifndef NDEBUG
    if (_gl_id == 0){
        throw MyIllegalStateException("Deleting a Texture without glGenTextures first.");
    }
    #endif

    glDeleteTextures(1, &_gl_id);
    fprintf(stdout, "glDeleteTextures()\n");
    fflush(stdout);
    _gl_id = 0;
}

Texture::Texture(Texture &&that){
//    printf("move constructor\n");
    _gl_id = that._gl_id;
    that._gl_id = 0;
}


Texture &&Texture::data(const char *file_name){
    if (_gl_id == 0) _gen();

    this->bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

//    typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXIMAGE2DPROC) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *data);
//    glCompressedTexImage2D()

//    unsigned char *pixels_data = stbi_load(file_name, &_width, &_height, &_nrChannels, 0);
    unsigned char *pixels_data = pvr_load(file_name, &_width, &_height, &_nrChannels);
    if (pixels_data == nullptr){
        throw MyGlException("Can not load the texture from file");
    }

//    GLAPI void GLAPIENTRY glTexImage2D (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA_S3TC_DXT5_EXT, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels_data);
    glGenerateMipmap(GL_TEXTURE_2D);
//    stbi_image_free(pixels_data);
    pvr_image_free(pixels_data);

    return (Texture&&)*this;
}


void Texture::bind()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _gl_id);
    glActiveTexture(GL_TEXTURE0);
}


Texture::~Texture(){
    _delete();
}
