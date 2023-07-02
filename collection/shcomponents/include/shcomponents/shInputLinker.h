#ifndef SH_INPUT_LINKER_H
#define SH_INPUT_LINKER_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <stdint.h>



#ifndef SH_MAX_HOST_MEMORY_COUNT
#define SH_MAX_HOST_MEMORY_COUNT 2048
#endif//SH_MAX_HOST_MEMORY_COUNT



typedef struct ShHostMemoryLinker {
	uint32_t host_memory_indices[SH_MAX_HOST_MEMORY_COUNT];
	uint8_t  active;
} ShHostMemoryLinker;



#ifdef __cplusplus
}
#endif//__cplusplus

#endif//SH_INPUT_LINKER_H