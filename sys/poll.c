#include <vga.h>
#include <io.h>
#include <stdint.h>
#include <string.h>

#include <cpu.h>

uint64_t rdtsc() {
	    uint64_t ret=0;
	        asm volatile ( "rdtsc" : "=r"(ret) );
		    return ret;
}

uint64_t get_control_register(int number)
{
	uint64_t ret=0;
	switch(number){
		case 0:
			asm volatile ( "mov %%cr0, %0" : "=r"(ret));
			break;
		case 2:
			asm volatile ( "mov %%cr2, %0" : "=r"(ret));
			break;
		case 3:
			asm volatile ( "mov %%cr3, %0" : "=r"(ret));
			break;
		case 4:
			asm volatile ( "mov %%cr4, %0" : "=r"(ret));
			break;
	}
    return ret;
}


void poll_init(){
	clear_screen();
	write_str("Poll Initializing...\r");
	cpu_init();
}
