#include <timer.h>
#define TIMER_IRQ 0
void timer_phase(int hz)
{
    int divisor = 1193180 / hz;       /* Calculate our divisor */
    outb(0x43, 0x36);             /* Set our command byte 0x36 */
    outb(0x40, divisor & 0xFF);   /* Set low byte of divisor */
    outb(0x40, divisor >> 8);     /* Set high byte of divisor */
}


/* This will keep track of how many ticks that the system
*  has been running for */
int timer_ticks = 0;

/* Handles the timer. In this case, it's very simple: We
*  increment the 'timer_ticks' variable every time the
*  timer fires. By default, the timer fires 18.222 times
*  per second. Why 18.222Hz? Some engineer at IBM must've
*  been smoking something funky */
long long seconds = 0;
long long minutes = 0;
long long hours = 0;
char* buff;
void timer_handler(struct regs *r){
    /* Increment our 'tick count' */

    timer_ticks++;
    cycles++;
    draw_str("Time elapsed:",0, 55);
    /* Every 100 clocks (approximately 1 second), we will
    *  display a message on the screen */
    if(timer_ticks % 100 == 0){
        seconds++;
    }
    if(timer_ticks % 6000 == 0){
        minutes++;
    }
    if(timer_ticks % 36000 == 0){
        hours++;
    }   
    if(seconds % 60 == 0){
        seconds = 0;
    }
    if(minutes % 60 == 0){
        minutes = 0;
    }
    if(hours % 24 == 0){
        hours = 0;
    }
        itoa(hours, 10, buff);
        draw_str(buff,0,70);
        draw_str(":",0,71);
        itoa(minutes, 10, buff);
        draw_str(buff,0,72);
        draw_str(":",0,73);
        itoa(seconds, 10, buff);
        draw_str(buff,0,74);
    irq_ack(TIMER_IRQ);
}

/* Sets up the system clock by installing the timer handler
*  into IRQ0 */
void timer_install(){
    /* Installs 'timer_handler' to IRQ0 */
    timer_phase(100);
    cycles = 0;
    irq_install_handler(TIMER_IRQ, timer_handler);
}
    
