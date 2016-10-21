#include <kernel/fos.h>
/*	TODO - bind Escape-sequences to write_char and draw_char
*	TODO - differentiate properly between CR NL LF etc.
*	TODO - implement proper function for log_print, printf(args)
*	TODO - try to refactor timer_wait, to see why it doesn't work
*/



void *memset(void *dest, char val, size_t count){
	char *temp = (char *)dest;
	for( ; count != 0; count--) *temp++ = val;
	return dest;
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
