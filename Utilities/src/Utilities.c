#include "Utilities.h"

#include <stdio.h>
#include <stdlib.h>

const char *ReadCode(const char* path, const char *mode) {
	
	FILE* file = fopen(path, mode);
	
	fseek(file, 0L, SEEK_END);
	long size = ftell(file);
	fseek(file, 0L, SEEK_SET);

	const char* code = (const char *)(malloc(size));

	if (file != NULL) {
		fread(code, size, 1, file);
		return code;
	}

	return (void*)(0);
}

