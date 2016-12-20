#ifndef TIMER_H
#define TIMER_H
#include <kernel/fos.h>

void timer_install();
void delay(int t);
void timer_phase(int hz);
#endif
