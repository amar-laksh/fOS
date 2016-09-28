#include <sys/vga.h>

#define	VIDMEM 0x000B8000
#define MAX_ROWS 24
#define MAX_COLUMNS 79
#define ERROR_CODE -12321
#define PASS_CODE 32123
#define PORT 0x09
#define RTC_PORT 0x70
int32_t x=1,y=2;
int32_t curpos=162;

int32_t get_row(uint32_t p){
	if(p>MAX_ROWS)
		return ERROR_CODE;
        return p*160;
}

int32_t get_column(uint32_t p){
	if(p>MAX_COLUMNS)
		return ERROR_CODE;
	return p*2;
}

int32_t get_point(uint32_t r, uint32_t c){
	return (get_row(r)+get_column(c));
}

void draw_char(uint32_t p, char ch, uint8_t fg, uint8_t bg) {
	char *fb = (char *) VIDMEM;
	fb[p] = ch;
	fb[p + 1] = ((fg & 0x0F) << 4) | (bg & 0x0F);
}

void clear_screen(){
        int32_t i=0;
        for(i=0;i<5000;i=i+2){
                draw_char(i,' ',0,0);
        }
}

int32_t draw_str(char *string, int32_t r, int32_t c){
	uint32_t i=0,j=0,sp=0;
	sp = get_point(r,c);
	for(i=sp;i<(sp+(strlen(string)*2));i=i+2){
			draw_char(i,string[j++],0,15);
	}
	return PASS_CODE;
}

uint64_t delay(uint64_t t){
	uint64_t st = 0;
	uint64_t i=0;
	uint64_t et = 0;
	st = 0;
	while(i<(t*1000000))
		i=i+1;
	et = i;
	return (et-st);
}


void write_char(char ascii){
	if(ascii == '\b'){
		if(curpos%160>3){
			curpos = curpos - 2;
			draw_char(curpos,' ',0,15);
			move_cursor(curpos/2);
		}
	}
	else if(ascii == '\r'){			
		while(curpos%160>0){
			curpos = curpos + 2;
		}
		draw_char(curpos,'#',0,15);
		move_cursor(curpos/2);
		curpos += 2;
	}
	else if(ascii == '\n'){
		while(curpos%160>0){
			curpos = curpos + 2;
		}
		move_cursor(curpos/2);
		curpos += 2;
	}
	else{
		draw_char(curpos,ascii,0,15);
		draw_char(curpos+2,' ',0,15);
		curpos = curpos + 2;
		move_cursor(curpos/2);
	}
}

void write_str(char* string){
	int32_t i=0;
	for(i=0;i<strlen(string);i++){
		write_char(string[i]);
	}
}


void vga_init(){
	char header[] = ".f.O.S. - By Amar Lakshya";
	draw_str(header,0,20);
	write_str("\rHello World! Currently the console provides the two commands:\n whoami, whoiscpu \r");
	while(read_scan_code()){
			write_char(get_kbd());
		}

}
