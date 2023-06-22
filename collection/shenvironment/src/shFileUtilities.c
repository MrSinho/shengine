#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include "shenvironment/shFileUtilities.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef _MSC_VER
#pragma warning (disable: 4996 6385)
#endif // _MSC_VER



const char* shReadCode(
	const char* path,
	const char* mode,
	uint32_t*   p_code_size
) {

	FILE* stream = fopen(path, mode);

	if (stream == NULL) { return NULL; }

	fseek(stream, 0, SEEK_END);
	uint32_t code_size = ftell(stream);
	fseek(stream, 0, SEEK_SET);

	char* code = (char*)malloc(code_size);
	if (code == NULL) { free(stream); return NULL; }

	fread(code, code_size, 1, stream);
	(p_code_size != NULL) && (*p_code_size = code_size);

	fclose(stream);

	return code;
}



#ifdef __cplusplus
}
#endif//__cplusplus