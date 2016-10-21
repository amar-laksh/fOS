#ifndef VGA_H
#define VGA_H
#include <stdint.h>
#include <stdbool.h>
#include <sys/io.h>
#include <drivers/serial.h>
#include <stdlib/string.h>
#include <sys/commands.h>
#include <timer.h>
#include <drivers/pcsound.h>

enum vga_color
{
	COLOR_BLACK = 0,
	COLOR_BLUE = 1,
	COLOR_GREEN = 2,
	COLOR_CYAN = 3,
	COLOR_RED = 4,
	COLOR_MAGENTA = 5,
	COLOR_BROWN = 6,
	COLOR_LIGHT_GREY = 7,
	COLOR_DARK_GREY = 8,
	COLOR_LIGHT_BLUE = 9,
	COLOR_LIGHT_GREEN = 10,
	COLOR_LIGHT_CYAN = 11,
	COLOR_LIGHT_RED = 12,
	COLOR_LIGHT_MAGENTA = 13,
	COLOR_LIGHT_BROWN = 14,
	COLOR_WHITE = 15,
};


int32_t get_row(uint32_t p);

int32_t get_column(uint32_t p);

int32_t get_point(uint32_t r, uint32_t c);

void clear_screen();

void delay(int64_t t);

char get_char_cell(int r, int c);

void draw_char(uint32_t p, char ch, uint8_t fg, uint8_t bg);

int32_t draw_str(char string[], int32_t r, int32_t c);

void write_char(char ascii);

void write_str(char* string);

void vga_init();

#endif
