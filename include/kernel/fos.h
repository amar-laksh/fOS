#ifndef FOS_H
#define FOS_H
#include <stdint.h>
#include <sys/io.h>
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


#endif
