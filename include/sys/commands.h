#ifndef COMMANDS_H
#define COMMANDS_H
#include <stdint.h>
#include <stdbool.h>
#include <sys/io.h>
#include <sys/vga.h>
#include <stdlib/string.h>
#include <drivers/serial.h>

void exec_cmd(int n, char buff[]);

#endif
