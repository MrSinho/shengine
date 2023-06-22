#ifndef SH_FILE_UTILITIES_H
#define SH_FILE_UTILITIES_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <stdint.h>



#define shReadBinary(path, p_code_size)\
	shReadCode(path, "rb", p_code_size)

#define shReadText(path, p_code_size)\
	shReadCode(path, "r", p_code_size)


extern const char* shReadCode(
	const char* path,
	const char* mode,
	uint32_t*   p_code_size
);



#ifdef __cplusplus
}
#endif//__cplusplus

#endif//SH_FILE_UTILITIES_H