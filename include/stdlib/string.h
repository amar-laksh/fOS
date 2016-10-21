#ifndef STRING_H
#define STRING_H
#include <stdint.h>

int32_t strlen(const char str[]);

void itoa(uint64_t i,uint32_t base, char* buf);

int8_t equals(char* first, char* second);

#endif
