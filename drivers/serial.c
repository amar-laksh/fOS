#include <drivers/serial.h>

serial_buffer serial;

void serial_install(int com_port, int baud_rate) {
   outb(com_port + 1, 0x00);
   outb(com_port + 3, 0x80);
   outb(com_port + 0, (baud_rate >> 8));
   outb(com_port + 1, (baud_rate << 8));
   outb(com_port + 3, 0x03);
   outb(com_port + 2, 0xC7);
   outb(com_port + 4, 0x0B);
   serial.COM_PORT = com_port;
   serial.BAUD_RATE = baud_rate;
}


int serial_received() {
   return inb(serial.COM_PORT + 5) & 1;
}
char read_serial() {
   while (serial_received() == 0);
   return inb(serial.COM_PORT);
}

int8_t is_transmit_empty() {
   return inb(serial.COM_PORT + 5) & 0x20;
}

void write_serial(char* a) {
	for(int i=0;i<strlen(a);i++){
		while (is_transmit_empty() == 0);
		outb(serial.COM_PORT,a[i]);
	}
}


