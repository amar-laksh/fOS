#ifndef SERIAL_H
#define SERIAL_H
#include <sys/io.h>
#include <sys/print.h>

typedef struct {
	int COM_PORT;
	int BAUD_RATE;
	int8_t works;
} serial_buffer;

serial_buffer serial;
void serial_install();

int serial_received();

char read_serial();

int8_t is_transmit_empty();

void write_serial(char* a);

#endif
