#include <kernel/fos.h>
/*
*	TODO - differentiate properly between CR NL LF etc.
*	TODO - try to refactor timer_wait, to see why it doesn't work
*/

extern void*  endKernel;

void *memset(void *dest, char val, size_t count){
	char *temp = (char *)dest;
	for( ; count != 0; count--) *temp++ = val;
	return dest;
}

void kmain(){
	mm_init(&endKernel);
	serial_install();
	gdt_install();
	idt_install();
	isrs_install();
	irq_install();
	timer_install();
	poll_init();
	pci_install();
	vga_init();
	for(;;)
		asm("hlt");

}
