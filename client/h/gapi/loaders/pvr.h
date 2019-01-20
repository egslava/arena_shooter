#ifndef PVR_H
#define PVR_H

unsigned char *pvr_load(const char *file_name, int *width, int *height, int *nrChannels);
void pvr_image_free(unsigned char *data);
#endif // PVR_H
