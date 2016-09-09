#ifndef IDT_H
#define IDT_H
#include <stdint.h>
#include <stddef.h>


struct idt_entry
{
    uint16_t base_low;
    uint16_t selector;        /* Our kernel segment goes here! */
    uint8_t always0;     /* This will ALWAYS be set to 0! */
    uint8_t flags;       /* Set using the above table! */
    uint16_t base_high;
} __attribute__((packed));

struct idt_ptr
{
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));


struct idt_entry idt[256];
struct idt_ptr idtp;

extern void idt_load();

void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);

void idt_install();

#endif
