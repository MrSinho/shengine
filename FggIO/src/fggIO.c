#include "fggIO.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma warning (disable: 4996)

void fggExport(FggIOSettings io, uint32_t src, const char* fn) {

	switch (src) {

	case FGG_IO_USB_SERIAL: 
		break;
	
	case FGG_IO_FILE: 

		FILE* stream = fopen(fn, "wb");
		if (stream == NULL) { return; }

		uint32_t offset = 0;

		for (uint32_t i = 0; i < io.attributeCount; i++) {
			
			fseek(stream, offset, SEEK_SET);

			fwrite((void*)((char*)io.pData + io.pAttributesStride[i]), io.pAttributesSize[i], 1, stream);
			offset += io.pAttributesSize[i];
		}

		break;

	}

}

void fggImport(FggIOSettings io, uint32_t src, const char* fn, void* dst) {

	switch (src) {

	case FGG_IO_USB_SERIAL:
		break;

	case FGG_IO_FILE:

		FILE* stream = fopen(fn, "rb");
		if (stream == NULL) { return; }

		uint32_t offset = 0;

		for (uint32_t i = 0; i < io.attributeCount; i++) {

			fseek(stream, offset, SEEK_SET);
			fread((void*)((char*)dst + io.pAttributesStride[i]), io.pAttributesSize[i], 1, stream);
			offset += io.pAttributesSize[i];

		}

		break;

	}

}