// TODO- REMEMBER DELAY DOESN'T WORK ON PCSOUND API (INTERUPT PRIORITY)
#include <kernel/fos.h>
void
timer_phase(int hz)
{
  int divisor = 1193180 / hz;
  outb(0x43, 0x36);
  outb(0x40, divisor & 0xFF);
  outb(0x40, divisor >> 8);
}

int timer_ticks = 0;

/* Handles the timer. In this case, it's very simple: We
 *  increment the 'timer_ticks' variable every time the
 *  timer fires. By default, the timer fires 18.222 times
 *  per second. Why 18.222Hz? Some engineer at IBM must've
 *  been smoking something funky */

char* buff;
void
timer_handler(irq_handler_t* handler)
{
  /* Increment our 'tick count' */
  timer_ticks++;
  cycles++;
  /* Every 100 clocks (approximately 1 second), we will
   *  display a message on the screen */
  if (cycles % 100 == 0) {
    seconds++;
  }
  if (cycles % 6000 == 0) {
    minutes++;
  }
  if (cycles % 360000 == 0) {
    hours++;
  }
  if (seconds % 60 == 0) {
    seconds = 0;
  }
  if (minutes % 60 == 0) {
    minutes = 0;
  }
  if (hours % 24 == 0) {
    hours = 0;
  }
  // draw_str("Console Buffer: ",10,50);
  // draw_str(term.buffer,11,50);
  draw_str("Time: ", 2, 55);
  itoa(hours, 10, buff);
  draw_str(buff, 2, 69);
  draw_str(":", 2, 71);
  itoa(minutes, 10, buff);
  draw_str(buff, 2, 72);
  draw_str(":", 2, 74);
  itoa(seconds, 10, buff);
  draw_str(buff, 2, 75);

  if (term.cursor >= 2060 && term.cursor <= 2074) {
    button_on = button_on == 1 ? 0 : 1;
  }

  term.color_fg_value = COLOR_RED;
  term.color_bg_value = COLOR_LIGHT_GREY;
  draw_str("MEM INFO", 12, 70);
  term.color_fg_value = COLOR_GREEN;
  term.color_bg_value = COLOR_BLACK;
  if (button_on == 1 && button_left == 1) {
    if (printed == 1) {
      draw_str("                         ", 5, 55);
      draw_str("                          ", 7, 55);
      draw_str("             ", 7, 71);

      draw_str("                            ", 8, 55);
      draw_str("              ", 8, 71);

      draw_str("                             ", 9, 55);
      draw_str("              ", 9, 71);
      printed = 0;
    } else {
      draw_str("Memory Information", 5, 55);
      draw_str("Memory used:         KB", 7, 55);
      itoa(memory_t.used_mem, 10, buff);
      draw_str(buff, 7, 71);

      draw_str("Memory un-used:        KB", 8, 55);
      itoa(memory_t.unused_mem, 10, buff);
      draw_str(buff, 8, 71);

      draw_str("Total Memory:         MB", 9, 55);
      itoa(memory_t.total_mem, 10, buff);
      draw_str(buff, 9, 71);
      printed = 1;
    }
  }

  draw_str("f.O.S. - Made By Amar Lakshya", 0, 20);
  irq_ack(TIMER_IRQ);
}

// TODO-  This is really hacky, remove it
void
delay(int t)
{
  int limit = 100000;
  int i = 0;
  while (i < (limit * t)) {
    draw_char(800, ' ', 0, 0);
    i++;
  }
  return;
}

/* Sets up the system clock by installing the timer handler
 *  into IRQ0 */
void
timer_install()
{
  /* Installs 'timer_handler' to IRQ0 */
  timer_phase(100);
  cycles = 0;
  seconds = 0;
  minutes = 0;
  hours = 0;
  irq_install_handler(TIMER_IRQ, timer_handler);
}
