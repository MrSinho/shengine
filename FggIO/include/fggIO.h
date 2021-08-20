#ifndef FGG_IO_H
#define FGG_IO_H

#include <stdint.h>

typedef struct FggIOSerialSettings {

	const char* comPort;
	uint32_t baudRate;

} FggIOSerialSettings;

typedef struct FggIOSettings {

	uint32_t  attributeCount;
	uint32_t* pAttributesSize;
	uint32_t* pAttributesStride;
	uint32_t* pBinAttributesStride;

} FggIOSettings;


extern void fggIOSerialSetup(const FggIOSerialSettings srl, const char* output);

extern void fggIOSerialRead();

extern void fggExport(const FggIOSettings io, void* src, const char* fn);

extern void fggImport(const FggIOSettings io, const char* fn, void* dst);

#endif