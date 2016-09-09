#include <stdint.h>
#include <stddef.h>

#include <gdt.h>
#include <idt.h>
#include <isrs.h>

#include <sys/poll.h>
#include <sys/vga.h>
#include <sys/io.h>


#if !defined(__cplusplus)
#include <stdbool.h>
#endif

 
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif
 
#if !defined(__i386__)
#error "This kernel needs to be compiled with a ix86-elf compiler"
#endif

/*	TODO - bind Escape-sequences to write_char and draw_char
*	TODO - differentiate properly between CR NL LF etc.
*	TODO - implement proper function for printing(somehow carry char* and int8_t* at the same time.)
*	TODO - implement proper function for delay
*	TODO - divide gdt, idt and isr codes into seperate .s files
*/
void kmain(){
	gdt_install();
	idt_install();
	isrs_install();

	poll_init();
	vga_init();

}
