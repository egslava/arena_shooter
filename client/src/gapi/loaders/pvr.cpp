#include "gapi.h"

#include "gapi/loaders/pvr.h"
#include <memory>

#pragma pack(4)
struct PVR_Header {
//    char version[4];
    union {
        char version[4];
        Uint32 _version;
    };
    Uint32 flags;

    // <pixel format>
    union {
        Uint32 zeros;  // reserved
        char order[4];
    } pixel_format_msb;

    union {
        Uint32 pixel_format;
        struct {
            Uint8 bits_per_ch1;
            Uint8 bits_per_ch2;
            Uint8 bits_per_ch3;
            Uint8 bits_per_ch4;
        }rgba;
    } pixel_format_lsb;

    // </pixel format>
    Uint32 colour_space;
    Uint32 channel_type;
    Uint32 height;
    Uint32 width;
    Uint32 depth;
    Uint32 num_surfaces;
    Uint32 num_faces;
    Uint32 mipmap_count;
    Uint32 metadata_size;
};
#pragma pack(0)

unsigned char *pvr_load(const char *file_name, int *width, int *height, int *nrChannels) {
    FILE *file = fopen(file_name, "rb");
    if (!file) {
        throw MyFileException("Can not open the file.");
    }

    PVR_Header header;
    memset(&header, 0, sizeof(PVR_Header));

    fread(&header, sizeof(header), 1, file);
#ifndef NDEBUG
    if (sizeof(PVR_Header) != 52) throw MyIllegalStateException("sizeof(PVR_Header) != 52");
    if (header._version != 0x03525650) throw MyFileException("Endianness doesn't match");
//    if (header.metadata_size != 0) throw MyFileException("Only compressed textures are supported!");
    if (header.flags != 0) throw MyFileException("Flags are not currently supported");
#endif

//    Uint32 data_size = header.metadata_size || (header.width * header.height);
    Uint32 bytes_per_pixel = header.pixel_format_lsb.rgba.bits_per_ch1;
    bytes_per_pixel += header.pixel_format_lsb.rgba.bits_per_ch2;
    bytes_per_pixel += header.pixel_format_lsb.rgba.bits_per_ch3;
    bytes_per_pixel += header.pixel_format_lsb.rgba.bits_per_ch4;
#ifndef NDEBUG
    if (bytes_per_pixel % 8 != 0) throw MyUnsupportedFileException("bytes per pixel should be aligned by 8");
#endif
    bytes_per_pixel /= 8;

    Uint32 data_size = header.width * header.height * bytes_per_pixel;
//    std::unique_ptr<unsigned char*> result()
//    std::mem

    fseek(file, header.metadata_size, SEEK_CUR);

    unsigned char *data = (unsigned char*) malloc(data_size);
    fread(data, data_size, 1, file);
    *width = header.width;
    *height = header.height;
//    malloc(&data)

    fclose(file);

    return data;
}

void pvr_image_free(unsigned char *data){
    free(data);
}
