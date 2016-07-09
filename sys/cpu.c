#include <vga.h>
#include <io.h>
#include <stdint.h>
#include <string.h>


void cpuid_get(int code) {
  uint32_t a; uint32_t d;
  asm volatile("cpuid":"=a"(a),"=d"(d):"a"(code):"ecx","ebx");
  char buff[32];
  itoa((d),2,buff);
  write_str("Stuff a:");
  write_str(buff);
  int32_t l = strlen(buff);
  write_str("\nThe length of the string is:");
  char buffer[32];
  itoa(l,10,buffer);
  write_str(buffer);	

}


void get_cpuid_string(int code, char* where) {
	asm volatile("cpuid":"=a"(*where),"=b"(*(where+1)),
               "=c"(*(where+2)),"=d"(*(where+3)):"a"(code));
}

