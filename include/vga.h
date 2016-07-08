#ifndef VGA_H
#define VGA_H
#include <stdint.h>
#include <stdbool.h>
#include <io.h>
#include <string.h>
int32_t get_row(uint32_t p);

int32_t get_column(uint32_t p);

int32_t get_point(uint32_t r, uint32_t c);

void clear_screen();

uint64_t delay(uint64_t t);

void draw_num(int64_t num, int32_t r, int32_t c);

void draw_char(uint32_t p, int8_t ch, uint8_t fg, uint8_t bg);

int32_t draw_str(int8_t string[], int32_t r, int32_t c);

void write_char(int8_t ascii);

void write_str(int8_t* string);

void vga_init();

#endif
