#ifndef POLL_H
#define POLL_H
#include <stdint.h>
#include <vga.h>
#include <io.h>

void cpuid(int code);

void get_cpuid_string(int code, char* where);

#endif