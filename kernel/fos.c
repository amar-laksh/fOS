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
	button_on = 0;
	button_left = 0;
	button_right = 0;
	printed = 0;
	clear_screen();
	
	gdt_install();
	kprintf("GDT initiated.\n", LOG);
	
	idt_install();
	kprintf("IDT initiated.\n", LOG);
	
	isrs_install();
	kprintf("ISRS initiated.\n", LOG);

	irq_install();
	kprintf("IRQ initiated.\n" , LOG);
	
	timer_install();
	kprintf("[%d]: TIMER initiated.\n", cycles, LOG);

	
	//serial_install();
	kprintf("[%d]: SERIAL initiated.\n", cycles, LOG);
	int total_mem = multiboot_check(mbd, magic);
	if(total_mem == -1){
		kprintf("FAILURE: MULTIBOOT INFO DIDN'T CHECK");
		return;
	}

	kprintf("[%d]: Multiboot check complete.\n", cycles, LOG);
	
	// TODO - This is a hacky solution, change it
	memory_t.mem_db = mbd;
	memory_t.magic = magic;

	mm_init((uint32_t)&endKernel, total_mem);
	kprintf("[%d]: MM initiated.\n", cycles, LOG);
	
	pci_install();
	kprintf("[%d]: PCI initiated.\n", cycles, LOG);

	elf_install();
	
	poll_init();
	kprintf("POLL initiated.\n");

	vga_init();
	asm("hlt");
}
