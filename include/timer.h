#ifndef TIMER_H
#define TIMER_H
#include <kernel/fos.h>
#include <sys/vga.h>

void timer_phase(int hz);

/* This will keep track of how many ticks that the system
*  has been running for */

void timer_wait(int ticks);


void timer_install();

#endif
