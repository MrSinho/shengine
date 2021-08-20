#include "fggIO.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#pragma warning (disable: 4996)

void fggIOSerialSetup(const FggIOSerialSettings srl, const char* output) {
	
	FILE* stream = fopen(output, "w");
	if (stream == NULL) { return; }

	char empty = "\n";
	fwrite(&empty, 1, 1, stream);
	fflush(stream);
	fclose(stream);


	fwrite(srl.comPort, 1, strlen(srl.comPort) - 1, stream);
	char baudRate[5];
	itoa(srl.baudRate, baudRate, 10);
	fwrite(baudRate, 1, strlen(srl.comPort) - 1, stream);
	fwrite(output, 1, strlen(output) - 1, stream);

	fflush(stream);
	fclose(stream);
}

void fggIOSerialRead() {
#ifdef _WIN32
	system("start /b ..\\Scripts\\fggSerialReader.bat &");
#else 
	system("bash ..\\Scripts\\fggSerialReader.bat");
#endif
}

void fggExport(const FggIOSettings io, void* src, const char* fn) {

	FILE* stream = fopen(fn, "wb");
	if (stream == NULL) { return; }

	uint32_t offset = 0;

	for (uint32_t i = 0; i < io.attributeCount; i++) {
		
		fseek(stream, offset, SEEK_SET);

		fwrite((void*)((char*)src + io.pAttributesStride[i]), io.pAttributesSize[i], 1, stream);
		offset += io.pAttributesSize[i];
	}

	fclose(stream);

}

void fggImport(const FggIOSettings io, const char* fn, void* dst) {

	FILE* stream = fopen(fn, "rb");
	if (stream == NULL) { return; }
	
	uint32_t offset = 0;
	
	for (uint32_t i = 0; i < io.attributeCount; i++) {
	
		offset = io.pBinAttributesStride[i];
		fseek(stream, offset, SEEK_SET);
		fread((void*)((char*)dst + io.pAttributesStride[i]), io.pAttributesSize[i], 1, stream);
		
	}

	fclose(stream);

}