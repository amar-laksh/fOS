#ifndef FOS_H
#define FOS_H
#include <stdint.h>
#include <io.h>
#include <stddef.h>



void  *memset(void *b, int c, int len) {

  uint8_t *p = b;
  while(len > 0) {
      *p = c;
      p++;
      len--;
    }
  return(b);
}

/* This defines what the stack looks like after an ISR was running */
struct regs {
    uint32_t gs, fs, es, ds;      /* pushed the segs last */
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
    uint32_t int_no, err_code;    /* our 'push byte #' and ecodes do this */
    uint32_t eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */ 
};


#endif
