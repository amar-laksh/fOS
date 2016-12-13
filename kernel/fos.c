#include <kernel/fos.h>
/*
*	TODO - try to refactor timer_wait, to see why it doesn't work
*/

extern void*  endKernel;

void *memset(void *dest, char val, size_t count){
	char *temp = (char *)dest;
	for( ; count != 0; count--) *temp++ = val;
	return dest;
}

void * memmove(void * restrict dest
				, const void * restrict src
				, size_t count) {
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
	term->cursor = 162;
	clear_screen();
	mm_init((uint32_t)&endKernel);
	kprintf("MM initiated.\n");
	serial_install(COM1_PORT,0x03);
	kprintf("SERIAL initiated.\n");
	pci_install();
	kprintf("PCI initiated.\n");
	gdt_install();
	kprintf("GDT initiated.\n");
	idt_install();
	kprintf("IDT initiated.\n");
	isrs_install();
	kprintf("ISRS initiated.\n");
	irq_install();
	kprintf("IRQ initiated.\n");
	timer_install();
	kprintf("TIMER initiated.\n");
	poll_init();
	kprintf("POLL initiated.\n");
	vga_init();
	asm("hlt");

}
