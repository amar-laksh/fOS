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
	mm_init(&endKernel);
	pci_install();
	serial_install();
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
