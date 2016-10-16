#ifndef VGA_H
#define VGA_H
#include <stdint.h>
#include <stdbool.h>
#include <sys/io.h>
#include <drivers/serial.h>
#include <stdlib/string.h>


int32_t get_row(uint32_t p);

int32_t get_column(uint32_t p);

int32_t get_point(uint32_t r, uint32_t c);

void clear_screen();

void delay(int64_t t);

void draw_num(int64_t num, int32_t r, int32_t c);

void draw_char(uint32_t p, char ch, uint8_t fg, uint8_t bg);

int32_t draw_str(char string[], int32_t r, int32_t c);

void write_char(char ascii);

void write_str(char* string);

void vga_init();

#endif
