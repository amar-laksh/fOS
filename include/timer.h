#ifndef TIMER_H
#define TIMER_H

#include <kernel/fos.h>
#include <sys/vga.h>
#include <irq.h>

void timer_wait(int ticks);


void timer_install();

#endif
