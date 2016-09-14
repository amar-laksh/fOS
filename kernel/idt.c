#include <idt.h>
#include <sys/vga.h>
#include <isrs.h>

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


/* Use this function to set an entry in the IDT. Alot simpler
*  than twiddling with the GDT ;) */
void idt_set_gate(uint8_t num, uint64_t base, uint16_t sel, uint8_t flags) {
	idt[num].base_low = (base & 0xFFFF);
	idt[num].base_high  = ( (base >> 16) & 0xFFFF);
	idt[num].selector = sel;
	idt[num].flags = flags;
	idt[num].always0 = 0;
}

/* Installs the IDT */
void idt_install()
{
    /* Sets the special IDT pointer up, just like in 'gdt.c' */
    idtp.limit = (sizeof (struct idt_entry) * 256) - 1;
    idtp.base = &idt;

    /* Clear out the entire IDT, initializing it to zeros */
    memset(&idt, 0, sizeof(struct idt_entry) * 256);

    /* Add any new ISRs to the IDT here using idt_set_gate */
    isrs_install(); 
    irq_install();



    /* Points the processor's internal register to the new IDT */
    idt_load();

//    while(1){write_str("IDT loaded!");}
}

