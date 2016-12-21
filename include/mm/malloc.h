#ifndef MALLOC_H_
#define MALLOC_H_

#include <kernel/fos.h>

typedef struct {
	uint8_t status;
	uint32_t size;
} alloc_t;

 void mm_init(uint32_t end, unsigned long total_mem);
 void mm_print_out();

 char* malloc(size_t size);
 void free(void *mem);

 void* memcpy(const void* dest, const void* src, size_t num );
// void* memset (void * ptr, int value, size_t num );
 void* memset16 (void *ptr, uint16_t value, size_t num);

#endif

