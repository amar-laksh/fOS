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
			asm volatile ( "call 1f \n\t"
					"1: pop %0" : "=r"(ret));
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
}

int32_t draw_str(char *string, int32_t r, int32_t c){
	uint32_t i=0,j=0,sp=0;
	sp = get_point(r,c);
	for(i=sp;i<(sp+(strlen(string)*2));i=i+2){
			draw_char(i,string[j++],0,15);
	}
	return PASS_CODE;
}

void delay(int64_t t){
	int c=0,i=0;
	while(c <= 999999999){
		for(i=0;i<99999999;i++)
			;
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
		int8_t c = 0;	
		while(curpos%160>0){
			curpos = curpos + 2;
		}
		draw_char(curpos,'#',0,15);
		draw_char(curpos+2,' ',0,15);
		curpos +=2;
		move_cursor(curpos/2);
		/*
		if(term->buffer[0] != ' '){
			while(c < term->offset){
				write_char(term->buffer[c]);
				c++;
			}
			null_buffer();
		}
		*/
	}
	else if(ascii == '\n'){
		while(curpos%160>0){
			curpos = curpos + 2;
		}
		move_cursor(curpos/2);
		curpos += 2;
	}
	else{
		term->buffer[term->offset] = ascii;
		term->offset += 1;
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


void null_buffer(){
	int8_t c = 0;
	while(c <= 100){
		term->buffer[c] = 0;
		c++;
	}
	term->offset=0;
}

void print_registers(){
	uint64_t eax_t, ebx_t, ecx_t, edx_t, eip_t;
	char eax[16], ebx[16], ecx[16], edx[16], eip[16];
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

	draw_str("System Registers: ",0,10);
	draw_str("EAX: ",1,10); draw_str(eax,1,15);
	draw_str("EBX: ",2,10); draw_str(ebx,2,15);
	draw_str("ECX: ",3,10); draw_str(ecx,3,15);
	draw_str("EDX: ",4,10); draw_str(edx,4,15);
	draw_str("EIP: ",5,10); draw_str(eip,5,15);
	draw_str("----------------------------------------",6,0);
}

void vga_init(){
	int i=0;
	char header[] = ".f.O.S. - By Amar Lakshya";
	draw_str(header,0,20);
	clear_screen();
	write_str("\r");
	write_str("\nHello World! Currently the console provides the two commands:\n whoami, whoiscpu \n");
	write_str("\r");
	while(read_scan_code()){
			print_registers();
			char l = get_kbd();
			write_char(l);
		}

}
