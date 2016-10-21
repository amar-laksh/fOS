#ifndef IDT_H
#define IDT_H
#include <stdint.h>
#include <stddef.h>
#include <sys/vga.h>
#include <isrs.h>

void idt_set_gate(uint8_t num, uint64_t base, uint16_t sel, uint8_t flags);

void idt_install();

#endif
