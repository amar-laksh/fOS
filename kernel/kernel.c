#include <poll.h>
#include <vga.h>
#include <io.h>
#include <stdint.h>
#include <gdt.h>

#if !defined(__cplusplus)
#include <stdbool.h>
#endif
#include <stddef.h>
#include <stdint.h>
 
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
*/
void kmain(){
	gdt_install();
	poll_init();
	vga_init();
}
