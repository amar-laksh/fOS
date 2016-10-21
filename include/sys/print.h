#include <kernel/fos.h>
#include <sys/va_list.h>


size_t vasprintf(char * buf, const char *fmt, va_list args);

int kprintf(const char *fmt, ...);

int sprintf(const char *fmt, ...);

int dprintf(const char *fmt, ...);