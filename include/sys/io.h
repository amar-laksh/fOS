#ifndef IO_H
#define IO_H
#include <stdint.h>
#include <stdbool.h>

#define FB_COMMAND_PORT 0x3D4
#define FB_DATA_PORT 0x3D5
#define FB_HIGH_BYTE_COMMAND 14
#define FB_LOW_BYTE_COMMAND 15
#define PIC1_PORT_A 0x20
#define PIC2_PORT_A 0xA0
#define PIC1_START_INTERRUPT 0x20
#define PIC2_START_INTERRUPT 0x28
#define PIC2_END_INTERRUPT PIC2_START_INTERRUPT + 7
#define PIC_ACK 0x20
#define KBD_DATA_PORT 0x60
#define PORT 0x40   /* COM1 */
#define KEY_DEVICE    0x60
#define KEY_PENDING   0x64
#define KEYBOARD_NOTICES 0
#define KEYBOARD_IRQ 1



typedef struct {
    char *frame_buffer;
    char buffer[100];
    int8_t offset;
    int cursor;
} console;

console *term;

typedef struct {
	char* keyboard_buffer;
	int keyboard_offset;
	char* mouse_buffer;
} io_buffer;


io_buffer *io_buff;


static inline void outb(uint16_t port, uint8_t val) {
	__asm__ __volatile__( "out %1, %0" : : "Nd"(port), "a"(val) );
}

static inline uint8_t inb(uint16_t port) {
	uint8_t ret;
	__asm__ __volatile__( "in %1, %0":  "=a"(ret): "Nd"(port) );
	return ret;
}

static inline void outl(uint16_t port, uint16_t val) {
	__asm__ __volatile__( "outl %%eax, %%dx" : : "Nd"(port), "a"(val) );
}

static inline uint32_t inl(uint16_t port) {
	uint16_t ret;
	__asm__ __volatile__( "inl %%dx, %%eax":  "=a"(ret): "Nd"(port) );
	return ret;
}

static inline void outs(uint16_t port, uint16_t val) {
	__asm__ __volatile__( "outl %1, %0" : : "Nd"(port), "a"(val) );
}

static inline uint32_t ins(uint16_t port) {
	uint16_t ret;
	__asm__ __volatile__( "inl %1, %0":  "=a"(ret): "Nd"(port) );
	return ret;
}



void keyboard_install();

void keyboard_reset_ps2();

void keyboard_wait();

void move_cursor(int32_t pos);

void getASCII(unsigned char c);


char readb(unsigned char c);

#endif
