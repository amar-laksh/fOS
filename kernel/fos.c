#include <kernel/fos.h>
extern void*  endKernel;


void kmain	(
			multiboot_info_t* mbd
			, unsigned int magic
			)
{
	term.color_fg_value = COLOR_GREEN;
	term.color_bg_value = COLOR_BLACK;
	term.cursor = 162;
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


	elf_install();
	poll_init();
	kprintf("POLL initiated.\n");

	vga_init();
	asm("hlt");
}
