#ifndef IRQ_H
#define IRQ_H
#include <kernel/fos.h>
#include <stdint.h>
#include <stddef.h>
#include <sys/io.h>
#include <idt.h>
#include <isrs.h>


typedef void (*irq_handler_t) (struct regs*);

void irq_install_handler(int irq, void (*handler)(struct regs *r));

void irq_uninstall_handler(int);

void irq_install();


#endif
