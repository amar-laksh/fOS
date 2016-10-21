#include <timer.h>

#define PIT_A 0x40
#define PIT_B 0x41
#define PIT_C 0x42
#define PIT_CONTROL 0x43

#define PIT_MASK 0xFF
#define PIT_SCALE 1193180
#define PIT_SET 0x36

#define TIMER_IRQ 0

#define SUBTICKS_PER_TICK 1000
#define RESYNC_TIME 1

/*
 * Set the phase (in hertz) for the Programmable
 * Interrupt Timer (PIT).
 */
void timer_phase(int hz) {
	int divisor = PIT_SCALE / hz;
	outb(PIT_CONTROL, PIT_SET);
	outb(PIT_A, divisor & PIT_MASK);
	outb(PIT_A, (divisor >> 8) & PIT_MASK);
}

/*
 * Internal timer counters
 */
uint64_t timer_ticks = 0;
uint64_t timer_subticks = 0;
int64_t timer_drift = 0;
int64_t _timer_drift = 0;

static int behind = 0;

/*
 * IRQ handler for when the timer fires
 */
int timer_handler(struct regs *r) {
	if (++timer_subticks == SUBTICKS_PER_TICK || (behind && ++timer_subticks == SUBTICKS_PER_TICK)) {
		timer_ticks++;
		timer_subticks = 0;
	}
	irq_ack(TIMER_IRQ);
	return 1;
}

/* This will continuously loop until the given time has
 * *  been reached */
void timer_wait(int ticks){
	uint64_t eticks;
	eticks = timer_ticks + ticks;
		    while(timer_ticks < eticks);
}

/*
 * Device installer for the PIT
 */
void timer_install(void) {
	irq_install_handler(TIMER_IRQ, timer_handler);
	timer_phase(SUBTICKS_PER_TICK); /* 100Hz */
}


