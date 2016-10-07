#include <kernel/fos.h>
#include <mouse.h>
/*	TODO - bind Escape-sequences to write_char and draw_char
*	TODO - differentiate properly between CR NL LF etc.
*	TODO - implement proper function for printing(somehow carry char* and int8_t* at the same time.)
*	TODO - implement proper function for delay
*/


#define PORT 0x3f8   /* COM1 */
void init_serial() {
   outb(PORT + 1, 0x00);    // Disable all interrupts
   outb(PORT + 3, 0x80);    // Enable DLAB (set baud rate divisor)
   outb(PORT + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
   outb(PORT + 1, 0x00);    //                  (hi byte)
   outb(PORT + 3, 0x03);    // 8 bits, no parity, one stop bit
   outb(PORT + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
   outb(PORT + 4, 0x0B);    // IRQs enabled, RTS/DSR set
}


int serial_received() {
   return inb(PORT + 5) & 1;
}
char read_serial() {
   while (serial_received() == 0);
   return inb(PORT);
}

int is_transmit_empty() {
   return inb(PORT + 5) & 0x20;
}

void write_serial(char* a) {
	for(int i=0;i<strlen(a);i++){
		while (is_transmit_empty() == 0);
		outb(PORT,a[i]);
	}
}


void *memset(void *dest, char val, size_t count){
	char *temp = (char *)dest;
	for( ; count != 0; count--) *temp++ = val;
	return dest;
}


 //Play sound using built in speaker
 static void play_sound(uint32_t nFrequence) {
 	uint32_t Div;
 	uint8_t tmp;
 
        //Set the PIT to the desired frequency
 	Div = 1193180 / nFrequence;
 	outb(0x43, 0xb6);
 	outb(0x42, (uint8_t) (Div) );
 	outb(0x42, (uint8_t) (Div >> 8));
 
        //And play the sound using the PC speaker
 	tmp = inb(0x61);
  	if (tmp != (tmp | 3)) {
 		outb(0x61, tmp | 3);
 	}
 }

 //make it shutup
 static void nosound() {
 	uint8_t tmp = inb(0x61) & 0xFC;
 
 	outb(0x61, tmp);
 }

void kmain(){
	gdt_install();
	idt_install();
	isrs_install();
	irq_install();
	timer_install();
	poll_init();
	vga_init();
	for(;;)
		asm("hlt");

}
