#include "ecs/shImage.h"
#include "stb_image.h"

//#ifndef NDEBUG
//#include "shCheck.h"
//#endif
//
//uint8_t shLoadImage(const char* path, ShImageChannels channels, ShImageInfo* p_image) {
//	p_image->raw = stbi_load(path, &p_image->width, &p_image->height, (int*)(&p_image->channels), channels);
//	p_image->size = p_image->width * p_image->height * sizeof(uint32_t);
//#ifndef NDEBUG
//	shCheckResult(p_image->size, SH_WARNING, "cannot load image at current path");
//#endif//NDEBUG
//	return (p_image->raw != NULL) ? 1 : 0;
//}