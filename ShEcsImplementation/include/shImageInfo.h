#ifndef SH_IMAGE_INFO_H
#define SH_IMAGE_INFO_H

#include <stdint.h>

typedef enum ShImageChannels { // copied from stb_image.h
	SH_IMAGE_CHANNEL_GREY		= 1,
	SH_IMAGE_CHANNEL_GREY_ALPHA = 2,
	SH_IMAGE_CHANNEL_RGB		= 3,
	SH_IMAGE_CHANNEL_RGBA		= 4
} ShImageChannels;

typedef struct ShImageInfo {
	const char*		path;
	uint32_t		width;
	uint32_t		height;
	uint32_t		size;
	uint8_t*		raw;
	ShImageChannels	channels;
} ShImageInfo;

#endif//SH_IMAGE_INFO_H
