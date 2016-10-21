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
int8_t cmd = 6;

char* commands[6] = {
	"clear",
	"exit",
	"whoami",
	"game",
	"hello",
	"serial"
};

typedef struct {
	char buffer[100];
	int8_t offset;
} console;

console *term;

uint64_t get_register(int number)
{
	uint64_t ret=0;
	switch(number){
		case 0:
			asm volatile ( "mov %%eax, %0" 
					: "=r"(ret));
			break;
		case 1:
			asm volatile ( "mov %%ebx, %0" 
					: "=r"(ret));
			break;
		case 2:
			asm volatile ( "mov %%ecx, %0" 
					: "=r"(ret));
			break;
		case 3:
			asm volatile ( "mov %%edx, %0" 
					: "=r"(ret));
			break;
		case 4:
			asm volatile ("1: movl $1b, %0" 
					: "=r" (ret));
			break;	
		case 5:
			asm volatile ("mov %%cs, %0"
					: "=r"(ret));
			break;
		case 6:
			asm volatile ("mov %%ds, %0"
					: "=r"(ret));
			break;
	}
    return ret;
}



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
	curpos = 2;
}

int32_t draw_str(char string[], int32_t r, int32_t c){
	uint32_t i=0,j=0,sp=0;
	sp = get_point(r,c);
	for(i=sp;i<(sp+(strlen(string)*2) )-1;i=i+2){
			draw_char(i,string[j++],0,15);
	}
	return PASS_CODE;
}

void delay(int64_t t){
	int c=0,i=0;
	while(c <= t){
		for(i=0;i<999;i++)
			draw_str("",0,100);
		c++;
	}
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
		draw_char(curpos+2,' ',0,15);
		curpos +=2;
		move_cursor(curpos/2);
		for(int i=1000;i<5000;i+=2000){
			play_sound(i);
			delay(10);
			nosound();
			delay(1);
		}
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
		curpos += 2;
		move_cursor(curpos/2);
	}
}

void write_str(char* string){
	int32_t i=0;
	for(i=0;i<strlen(string);i++){
		write_char(string[i]);
	}
}

void print_registers(){
		uint64_t eax_t, ebx_t, ecx_t, edx_t, eip_t, cs_t, ds_t;
		char eax[16], ebx[16], ecx[16], edx[16], eip[16],cs[16],ds[16];
		eax_t = get_register(0);
		itoa(eax_t,16,eax);
		
		ebx_t = get_register(1);
		itoa(ebx_t,16,ebx);

		ecx_t = get_register(2);
		itoa(ecx_t,16,ecx);

		edx_t = get_register(3);
		itoa(edx_t,16,edx);

		eip_t = get_register(4);
		itoa(eip_t,16,eip);

		cs_t = get_register(5);
		itoa(cs_t,16,cs);

		ds_t = get_register(6);
		itoa(ds_t,16,ds);

		draw_str("System Registers: ",0,50);
		draw_str("EAX: ",1,50); draw_str(eax,1,55);
		draw_str("EBX: ",2,50); draw_str(ebx,2,55);
		draw_str("ECX: ",3,50); draw_str(ecx,3,55);
		draw_str("EDX: ",4,50); draw_str(edx,4,55);
		draw_str("EIP: ",5,50); draw_str(eip,5,55);
		draw_str("CS:  ",6,50); draw_str(cs,6,55);
		draw_str("DS:  ",7,50); draw_str(ds,7,55);
		draw_str("------------------------------",8,50);
}


void null_buffer(){
	int c = 0;
	while(c < 100){
		term->buffer[c] = '\0';
		c++;
	}
	term->offset=0;

}

void append_buffer(char l){
	if(l == '\b')
		term->buffer[--term->offset] = 0;
	else if(l !='\r'){
		term->buffer[term->offset++] = l;
	}
}

int process_buffer(){
	char b[2];
	int8_t c=0;
	itoa(term->offset,10,b);
	draw_str("Console offset: ",12,50);
	draw_str(b,13,50);
	for(int i=0; i<cmd;i++){
		if(equals(commands[i],term->buffer) == 0){
				exec_cmd(i, term->buffer);
				null_buffer();
				c++;
		}
	}
	if(c == 0){
		exec_cmd(cmd+1, term->buffer);
		null_buffer();
	}

	return 0;
}

void vga_init(){
	int code=0;
	curpos=162;
	clear_screen();
	draw_str("f.O.S. - Made By Amar Lakshya",0,20);
	write_str("\nHello World!\nThe console provides the following commands:\n whoami, hello, exit, clear \n");
	null_buffer();
	write_str("\r");
	while(read_scan_code()){
			print_registers();
			char l = get_kbd();
			draw_str("f.O.S. - Made By Amar Lakshya",0,20);
			append_buffer(l);
			if(l=='\r')
				code = process_buffer();
			if(code == -999)
				break;
			draw_str("Console Buffer: ",10,50);
			draw_str(term->buffer,11,50);
			write_char(l);
		}

}
