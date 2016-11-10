#ifndef TIMER_H
#define TIMER_H
#include <sys/vga.h>
#include <kernel/fos.h>

void timer_install();
void timer_wait(int ticks);
void timer_phase(int hz);
#endif
