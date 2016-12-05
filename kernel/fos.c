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

void * memmove(void * restrict dest, const void * restrict src, size_t count) {
	size_t i;
	unsigned char *a = dest;
	const unsigned char *b = src;
	if (src < dest) {
		for ( i = count; i > 0; --i) {
			a[i-1] = b[i-1];
		}
	} else {
		for ( i = 0; i < count; ++i) {
			a[i] = b[i];
		}
	}
	return dest;
}

void kmain(){
	mm_init((uint32_t)&endKernel);
	draw_str("MM initiated",1,1);
	serial_install(COM1_PORT,0x03);
	draw_str("SERIAL initiated",2,1);
	pci_install();
	draw_str("PCI initiated",3,1);
	gdt_install();
	draw_str("GDT initiated",4,1);
	idt_install();
	draw_str("IDT initiated",5,1);
	isrs_install();
	draw_str("ISRS initiated",6,1);
	irq_install();
	draw_str("IRQ initiated",7,1);
	timer_install();
	draw_str("TIMER initiated",8,1);
	poll_init();
	draw_str("POLL initiated",9,1);
	delay(10);
	vga_init();
	asm("hlt");

}
