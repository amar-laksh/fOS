#ifndef SERIAL_H
#define SERIAL_H
#include <sys/io.h>
#define COM1_PORT 0x3F8
#define COM2_PORT 0x2F8
#define COM3_PORT 0x3E8
#define COM4_PORT 0x2F8

typedef struct {
	int COM_PORT;
	int BAUD_RATE;
} serial_buffer;

void serial_install(int comp_port, int baud_rate);

int serial_received();

char read_serial();

int8_t is_transmit_empty();

void write_serial(char* a);

#endif
