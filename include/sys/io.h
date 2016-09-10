#ifndef IO_H
#define IO_H
#include <stdint.h>
#include <stdbool.h>


static inline void outb(uint16_t port, uint8_t val) {
	__asm__ __volatile__( "out %1, %0" : : "Nd"(port), "a"(val) );
}

static inline uint8_t inb(uint16_t port) {
	uint8_t ret;
	__asm__ __volatile__( "in %1, %0":  "=a"(ret): "Nd"(port) );
	return ret;
}


void kbd_set_leds (bool num, bool caps, bool scroll);

void pic_acknowledge(uint32_t interrupt);

int8_t read_scan_code(void);

int8_t readb(int8_t port);

void move_cursor(int32_t pos);

int8_t get_kbd();



#endif
