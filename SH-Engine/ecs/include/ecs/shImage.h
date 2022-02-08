#ifndef SH_IMAGE_H
#define SH_IMAGE_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <stdint.h>

typedef enum ShImageChannels ShImageChannels;

typedef struct ShImageInfo ShImageInfo;

extern uint8_t shLoadImage(const char* path, ShImageChannels channels, ShImageInfo* p_image);

#ifdef __cplusplus
}
#endif//__cplusplus

#endif//SH_IMAGE_H