#include <vga.h>
#include <io.h>
#include <stdint.h>

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

void get_cpuid_string(int code, int8_t* where) {
	asm volatile("cpuid":"=a"(*where),"=b"(*(where+1)),
               "=c"(*(where+2)),"=d"(*(where+3)):"a"(code));
}

void poll_init(){
	clear_screen();
	int8_t debug[] = "poll initializing...";
	int8_t cpuid[10];
	int8_t cpumsg[20] = "CPUID is: ";
	write_str(debug);
	write_str(cpumsg);
	get_cpuid_string(0,cpuid);
	write_str(cpuid);

}
