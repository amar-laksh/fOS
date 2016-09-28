#ifndef IRQ_H
#define IRQ_H
#include <kernel/fos.h>
#include <stdint.h>
#include <stddef.h>
#include <sys/io.h>
#include <idt.h>
#include <isrs.h>

#define IRQ_OFF { asm volatile ("cli"); }
#define IRQ_RES { asm volatile ("sti"); }
#define PAUSE { asm volatile ("hlt"); }
#define IRQS_ON_AND_PAUSE { asm volatile ("sti\nhlt\ncli"); }
#define STOP while (1) { PAUSE; }


typedef void (*irq_handler_t) (struct regs*);

void irq_install_handler(int irq, void (*handler)(struct regs *r));

void irq_uninstall_handler(int);

void irq_install();


#endif
