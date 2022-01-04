#ifndef SH_IMAGE_H
#define SH_IMAGE_H

#include <stdint.h>

typedef enum ShImageChannels ShImageChannels;

typedef struct ShImageInfo ShImageInfo;

extern uint8_t shLoadImage(const char* path, ShImageChannels channels, ShImageInfo* p_image);

#endif//SH_IMAGE_H