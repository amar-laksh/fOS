#ifndef SERIAL_LIST_H
#define SERIAL_LIST_H
#define COM1_PORT 0x3F8
#define COM2_PORT 0x2F8
#define COM3_PORT 0x3E8
#define COM4_PORT 0x2F8

typedef struct {
	int COM_PORT;
	int BAUD_RATE;
	int STATUS;
} serial_t;

serial_t serial_table[] =
{
	{COM1_PORT, 0x03, 0x00},
	{COM2_PORT, 0x03, 0x00},
	{COM3_PORT, 0x03, 0x00},
	{COM4_PORT, 0x03, 0x00}
};

#endif