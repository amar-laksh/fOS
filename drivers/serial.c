// TODO - Optimize the serial_install() code
#include <drivers/serial.h>
#include <drivers/serial_list.h>
serial_buffer serial;


int8_t loopback_test(int com_port){
   char t = 0xAA;
   outb(com_port+7,t);
   char l =  inb(com_port+7);
   if(l == t)
      return 1;
   else
      return -1;
}

int8_t serial_enable(int com_port, int baud_rate){
   outb(com_port + 1, 0x00);
   outb(com_port + 3, 0x80);
   outb(com_port + 0, (baud_rate >> 8));
   outb(com_port + 1, (baud_rate << 8));
   outb(com_port + 3, 0x03);
   outb(com_port + 2, 0xC7);
   outb(com_port + 4, 0x0B);
   if(loopback_test(com_port) < 0){
      return -1;
   }
   serial.COM_PORT = com_port;
   serial.BAUD_RATE = baud_rate;
   return 0;
}


void serial_install() {
   int8_t f = 12, c = 0;
   for(unsigned long i=0;i<4;i++){
      f = serial_enable(serial_table[i].COM_PORT, 0x03);
      if(f == -1)
         serial_table[i].STATUS = 0x00;
      else{
         serial_table[i].STATUS = 0xFF;
         c++;
      }
   }
   if(c == 0){
      kprintf("Error: no Serial port detected (loopback test failed)\n");
      for (int i = 0; i < 999999999; ++i)
      {}
   }
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


