#ifndef ISRS_H
#define ISRS_H
#include <kernel/fos.h>
#include <stdint.h>
#include <stddef.h>
#include <idt.h>
/* This defines what the stack looks like after an ISR was running */


void isrs_install();

void fault_handler(struct regs *r);


#endif
