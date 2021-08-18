#ifndef FGG_IO_H
#define FGG_IO_H

#include <stdint.h>


#define FGG_IO_USB_SERIAL	0
#define FGG_IO_FILE			1



typedef struct FggIOSettings {

	uint32_t attributeCount;
	uint32_t* pAttributesSize;
	uint32_t* pAttributesStride;

	void* pData;

} FggIOSettings;



extern void fggExport(FggIOSettings io, uint32_t src, const char* fn);

extern void fggImport(FggIOSettings io, uint32_t src, const char* fn, void* dst);

#endif