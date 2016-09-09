#include <stdlib/conv.h>

void itoi(uint64_t i,uint32_t base, uint64_t* buf) {
	uint64_t tbuf[100];
	uint64_t bchars[] = {0,1,2,3,4,5,6,7,8,9};
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
