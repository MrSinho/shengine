#ifndef SH_FILE_H
#define SH_FILE_H

#include <stdint.h>

#define shReadBinary(path, p_code_size)\
	shReadCode(path, "rb", p_code_size)

#define shReadText(path, p_code_size)\
	shReadCode(path, "r", p_code_size)


extern const char* shReadCode(const char* path, const char* mode, uint32_t* p_code_size);

#endif//SH_FILE_H