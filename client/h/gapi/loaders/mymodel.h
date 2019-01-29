#ifndef MYMODEL_H
#define MYMODEL_H

namespace MyModel {
    struct VBOs {
        std::vector<float> pos, tex, nor;
    };

    VBOs load(const char *filename);
    void free(VBOs vbos);
};

#endif // MYMODEL_H
