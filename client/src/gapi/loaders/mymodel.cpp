#include "gapi/gapi.h"
#include "gapi/loaders/mymodel.h"
#include <cstdint>

using namespace std;

enum class Has: Uint32 {
    POS   = 1 << 0,
    TEX   = 1 << 1,
    NOR   = 1 << 2
};

#pragma pack(4)
struct Header {
    Has flags;
    Uint32 n_triangles;
};


namespace MyModel {
    void _load_buffer(Header header,  Has _flag, FILE* from, std::vector<float> &to, uint floats_per_tri ){
        uint flag = static_cast<uint>(_flag);
        uint flags = static_cast<uint>(header.flags);

        if ((flags & flag) == 0) return;

        size_t size = header.n_triangles * 3 * floats_per_tri * sizeof(float);

        to.resize(header.n_triangles * 3 * floats_per_tri);
//        to = new float[header.n_triangles * floats_per_tri ];
        if(!fread(to.data(), size, 1, from)){
            throw MyFileException("Can not fread the model file");
        }
    }

    VBOs load(const char *filename){
        FILE *file = fopen(filename, "rb");
        if (!file) throw MyFileException("Can not open a model's file");

        Header header;

        if (! fread(&header, sizeof(Header), 1, file)) {
            throw MyFileException("Can not fread the model file");
        };

        VBOs vbos;

        _load_buffer(header, Has::POS, file, vbos.pos, 3);
        _load_buffer(header, Has::TEX, file, vbos.tex, 2);
        _load_buffer(header, Has::NOR, file, vbos.nor, 3);

//        fpos_t pos;
//        fgetpos(file, &pos);
//        printf("FILE POS: %d", pos);

        fclose(file);
        return vbos;
    }

    void free(VBOs vbos){
//        if (vbos.pos != 0) delete[] vbos.pos;
//        if (vbos.tex != 0) delete[] vbos.tex;
//        if (vbos.nor != 0) delete[] vbos.nor;
    }



}
