#include <kernel/fos.h>

/*	TODO - bind Escape-sequences to write_char and draw_char
*	TODO - differentiate properly between CR NL LF etc.
*	TODO - implement proper function for printing(somehow carry char* and int8_t* at the same time.)
*	TODO - implement proper function for delay
*	TODO - divide gdt, idt and isr codes into seperate .s files
*/

void *memset(void *dest, char val, size_t count){
	char *temp = (char *)dest;
	for( ; count != 0; count--) *temp++ = val;
	return dest;
}


void kmain(){
	//gdt_install();
	//idt_install();
	//timer_install();
	poll_init();
	vga_init();
}
