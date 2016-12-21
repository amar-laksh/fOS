#ifndef FOS_H
#define FOS_H
#include <stdint.h>
#include <stddef.h>

#if !defined(__cplusplus)
#include <stdbool.h>
#endif

 
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif
 
#if !defined(__i386__)
#error "This kernel needs to be compiled with a ix86-elf compiler"
#endif

#include <kernel/multiboot.h>
#include <mm/malloc.h>
#include <gdt.h>
#include <idt.h>
#include <isrs.h>
#include <irq.h>
#include <timer.h>


#include <sys/poll.h>
#include <sys/vga.h>
#include <sys/io.h>
#include <sys/print.h>
#include <drivers/serial.h>
#include <drivers/pcsound.h>

#define IRQ_OFF { asm volatile ("cli"); }
#define IRQ_RES { asm volatile ("sti"); }
#define PAUSE { asm volatile ("hlt"); }
#define IRQS_ON_AND_PAUSE { asm volatile ("sti\nhlt\ncli"); }
#define STOP while (1) { PAUSE; }


long long cycles;
long long seconds;
long long minutes;
long long hours;
struct regs {
    uint32_t gs, fs, es, ds;      /* pushed the segs last */
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */

    uint32_t int_no, err_code;    /* our 'push byte #' and ecodes do this */
    uint32_t eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */ 
};


void *memset(void *dest, char val, size_t count);

void * memmove(void * restrict dest, const void * restrict src, size_t count);

void kmain();

#endif
