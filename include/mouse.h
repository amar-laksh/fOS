#ifndef MOUSE_H
#define MOUSE_H

#include <stdint.h>
#include <stddef.h>
#include <kernel/fos.h>

//Mouse functions
void mouse_handler(struct regs *a_r);

inline void mouse_wait(uint8_t a_type);

inline void mouse_write(uint8_t a_write);


uint8_t mouse_read();


void mouse_install();

#endif
