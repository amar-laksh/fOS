#ifndef FOS_H
#define FOS_H
#include<stdint.h>
#include<io.h>

void abort(void){
	clear_screen();
        asm volatile ( "hlt" );
	asm volatile ("ret");
}


#endif
