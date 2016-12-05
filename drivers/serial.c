#include <drivers/serial.h>

serial_buffer serial;

void serial_install(int com_port, int baud_rate) {
   outb(com_port + 1, 0x00);    // Disable all interrupts
   outb(com_port + 3, 0x80);    // Enable DLAB (set baud rate divisor)
   outb(com_port + 0, (baud_rate >> 8));    // Set divisor to 3 (lo byte) 38400 baud
   outb(com_port + 1, (baud_rate << 8));    //                  (hi byte)
   outb(com_port + 3, 0x03);    // 8 bits, no parity, one stop bit
   outb(com_port + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
   outb(com_port + 4, 0x0B);    // IRQs enabled, RTS/DSR set
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


