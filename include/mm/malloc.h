#ifndef MALLOC_H_
#define MALLOC_H_

#include <kernel/fos.h>

typedef struct {
	uint8_t status;
	uint32_t size;
} alloc_t;

extern void mm_init();
extern void mm_print_out();

extern char* malloc(size_t size);
extern void free(void *mem);

extern void* memcpy(const void* dest, const void* src, size_t num );
extern void* memset (void * ptr, int value, size_t num );
extern void* memset16 (void *ptr, uint16_t value, size_t num);

#endif

