#include <timer.h>


void timer_handler(struct regs *r)
{

    timer_ticks++;
    write_str("in timer handler");
    if (timer_ticks % 18 == 0)
    {
        write_str("One second has passed\n");
    }
}

void timer_wait(int ticks)
{
    uint64_t eticks;

    eticks = timer_ticks + ticks;
    while(timer_ticks < eticks);
}



void timer_install() {
	write_str("in timer install");
	irq_install_handler(0, timer_handler);
}

