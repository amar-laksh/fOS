#ifndef SERIAL_H
#define SERIAL_H
#include <sys/io.h>
#define COM1_PORT 0x3f8   /* COM1 */


void serial_install();

int serial_received();

char read_serial();

int8_t is_transmit_empty();

void write_serial(char* a);

#endif
