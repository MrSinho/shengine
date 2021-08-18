#include "fggIO.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma warning (disable: 4996)

void fggExport(FggIOSettings io, uint32_t dst, const char* fn) {

	switch (dst) {

	case FGG_IO_USB_SERIAL: 
		break;
	
	case FGG_IO_FILE: 

		FILE* stream = fopen(fn, "wb");

		uint32_t offset = 0;

		for (uint32_t i = 0; i < io.attributeCount; i++) {
			
			fseek(stream, offset, SEEK_SET);
			fwrite(io.ppData + io.pAttributesStride[i], 1, io.pAttributesSize[i], stream);
			offset += io.pAttributesSize[i];
		}

		break;

	}

}

void fggImport(FggIOSettings io, uint32_t src, void* dst) {



}