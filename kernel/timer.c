#include <timer.h>


/* This will keep track of how many ticks that the system
*  has been running for */
int timer_ticks = 0;

void timer_handler(struct regs *r) {
    timer_ticks++;
    write_str("in timer handler");
    if (timer_ticks % 18 == 0)
    {
        write_str("One second has passed\n");
    }
}

void timer_wait(int ticks)
{
    int64_t eticks;

    eticks = timer_ticks + ticks;
    while(timer_ticks < eticks);
}



void timer_install() {
	irq_install_handler(0, timer_handler);
}

