#include <sys/poll.h>

uint64_t rdtsc() {
	    uint64_t ret=0;
	        __asm__ __volatile__ ( "rdtsc" : "=r"(ret) );
		    return ret;
}


void poll_init(){
	cpu_init();
}
