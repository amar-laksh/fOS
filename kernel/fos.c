#include <kernel/fos.h>

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
	set_clock();
	clear_screen();
	mm_init((uint32_t)&endKernel);
	kprintf("MM initiated.\n");
	serial_install();
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
	delay(500);
	kprintf("POLL initiated.\n");
	for(;;);
	vga_init();
	asm("hlt");

}
