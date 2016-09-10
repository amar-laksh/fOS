#ifndef GDT_H
#define GDT_H
#include <stdint.h>
#include <stddef.h>

void gdt_set_gate(int num, uint64_t base, uint64_t limit, uint8_t access, uint8_t gran);

void gdt_install();

#endif
