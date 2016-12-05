#ifndef CONV_H
#define CONV_H
#include <stdint.h>
#include <sys/vga.h>
#include <sys/io.h>


void itoi(uint64_t i,uint32_t base, uint64_t* buf);

int atoi(char str[]);

char* join(char* buff[5]);

int pow(int base, int exponent);

#endif
