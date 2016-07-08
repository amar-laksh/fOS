#ifndef POLL_H
#define POLL_H
#include <stdint.h>
#include <vga.h>
#include <io.h>

uint64_t rdtsc();

uint64_t get_control_register(int number);

void get_cpuid_string(int code, int8_t* where);

void poll_init();

#endif
