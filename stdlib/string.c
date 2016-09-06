#include <stdint.h>
#include <string.h>

int32_t strlen(char str[]){
	int32_t len=0;
	while (str[len++]);
	return len;
}

void itoa(uint64_t i,uint32_t base, char* buf) {
	char tbuf[100];
	char bchars[] = {'0','1','2','3','4','5',
			 '6','7','8','9','A','B',
			 'C','D','E','F'};
	int32_t pos = 0;
	int32_t opos = 0;
	int32_t top = 0;

	if (i == 0 || base > 16) {
		buf[0] = '0';
		buf[1] = '\0';
		return;
	}

	while (i != 0) {
		tbuf[pos] = bchars[i % base];
		pos++;
		i /= base;
	}
	top=pos--;
	for (opos=0; opos<top; pos--,opos++)
		buf[opos] = tbuf[pos];
	buf[opos] = 0;
}


