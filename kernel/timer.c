// TODO- REMEMBER DELAY DOESN'T WORK ON PCSOUND API (INTERUPT PRIORITY)
#include <kernel/fos.h>
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

char* buff;
void timer_handler(irq_handler_t * handler){
    /* Increment our 'tick count' */
    timer_ticks++;
    cycles++;
    /* Every 100 clocks (approximately 1 second), we will
    *  display a message on the screen */
    if(cycles % 100 == 0){
        seconds++;
    }
    if(cycles % 6000 == 0){
        minutes++;
    }
    if(cycles % 360000 == 0){
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
    draw_str("Console Buffer: ",10,50);
    draw_str(term->buffer,11,50);
    draw_str("Time: ",2, 55);
    itoa(hours, 10, buff);
    draw_str(buff,2,69);
    draw_str(":",2,71);
    itoa(minutes, 10, buff);
    draw_str(buff,2,72);
    draw_str(":",2,74);
    itoa(seconds, 10, buff);
    draw_str(buff,2,75);

    // Check if the game has begun or not
    draw_str("f.O.S. - Made By Amar Lakshya",0,20);
    irq_ack(TIMER_IRQ);
}

void delay(int t){
    long long st = t + cycles;
    while((cycles) < st){
        draw_char(800,' ',0,0); //TODO-  This is really hacky, remove it
    }
    return;
}

/* Sets up the system clock by installing the timer handler
*  into IRQ0 */
void timer_install(){
    /* Installs 'timer_handler' to IRQ0 */
    timer_phase(100);
    cycles = 0;
    seconds = 0;
    minutes = 0;
    hours = 0;
    irq_install_handler(TIMER_IRQ, timer_handler);
}
    
