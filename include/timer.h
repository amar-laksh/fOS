#ifndef TIMER_H
#define TIMER_H

#include <sys/vga.h>
#include <irq.h>

/* This will keep track of how many ticks that the system
*  has been running for */
uint32_t timer_ticks = 0;

/* Handles the timer. In this case, it's very simple: We
*  increment the 'timer_ticks' variable every time the
*  timer fires. By default, the timer fires 18.222 times
*  per second. Why 18.222Hz? Some engineer at IBM must've
*  been smoking something funky */
void timer_handler(struct regs *r);

/* This will continuously loop until the given time has
*  been reached */
void timer_wait(int ticks);

/* Sets up the system clock by installing the timer handler
*  into IRQ0 */
void timer_install();

#endif
