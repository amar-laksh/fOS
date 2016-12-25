// TODO - Write unit tests for the entire kernel
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

void write(int fd, const void* buf, size_t count){
	__asm__ __volatile__ ("mov %0,%%edx"::"d"(count));
	__asm__ __volatile__ ("mov %0,%%ecx"::"c"(buf));
	__asm__ __volatile__ ("mov %0,%%ebx"::"b"(fd));
	__asm__ __volatile__ ("mov %0,%%eax"::"a"(11));
	__asm__ __volatile__("int $69");
}

void kmain(multiboot_info_t* mbd, unsigned int magic){
	term->cursor = 162;
	clear_screen();
		gdt_install();
	kprintf("GDT initiated.\n");
	
	idt_install();
	kprintf("IDT initiated.\n");
	
	isrs_install();
	kprintf("ISRS initiated.\n");
	
	serial_install();
	kprintf("SERIAL initiated.\n");
	int total_mem = multiboot_check(mbd, magic);
	if(total_mem == -1){
		kprintf("FAILURE: MULTIBOOT INFO DIDN'T CHECK");
		return;
	}
	kprintf("Multiboot check complete.\n");
	// TODO - This is a hacky solution, change it
	memory_t.mem_db = mbd;
	memory_t.magic = magic;

	mm_init((uint32_t)&endKernel, total_mem);
	kprintf("MM initiated.\n");
	pci_install();
	kprintf("PCI initiated.\n");

	irq_install();
	kprintf("IRQ initiated.\n");
	
	timer_install();
	kprintf("TIMER initiated.\n");

	poll_init();
	kprintf("POLL initiated.\n");
	vga_init();
	asm("hlt");
}