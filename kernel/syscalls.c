#include<kernel/fos.h>

size_t sys_write(int fd, const void *buf, size_t count){
	const char* buffer = buf;
	if(fd == 0){
		clear_screen();
		for (unsigned long i = 0 ; i < count; i++){
			write_char(buffer[i], COLOR_BLACK, COLOR_GREEN);
		}
	}
	else{
		for(unsigned long i=0;i<count;i++){
			while (is_transmit_empty() == 0)
				;
				outb(serial.COM_PORT,buffer[i]);
		}
	}
	return 0;
} 
