#ifndef POLL_H
#define POLL_H
#include <stdint.h>
#include <sys/vga.h>
#include <sys/io.h>
#include <sys/cpu.h>
uint64_t rdtsc();

uint64_t get_control_register(int number);

void get_cpuid_string(int code, char* where);

void poll_init();

#endif
