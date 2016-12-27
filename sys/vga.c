// TODO - Completely refactor this shitty code
#include <kernel/fos.h>


int32_t get_row(uint32_t p){
	if(p>MAX_ROWS)
		return ERROR_CODE;
    return p*((MAX_COLUMNS+1)*2);
}

int32_t get_column(uint32_t p){
	if(p>MAX_COLUMNS)
		return ERROR_CODE;
	return p*2;
}

int32_t get_point(uint32_t r, uint32_t c){
	return (get_row(r)+get_column(c));
}

char get_char_cell(int r, int c){
	char *fb = (char *) VIDMEM;
	return fb[get_point(r,c)]; 
}

void draw_char(uint32_t p
				, char ch
				, uint8_t fg
				, uint8_t bg) {
	char *fb = (char *)  VIDMEM;
	fb[p] = ch;
	fb[p + 1] = ((fg & 0x0F) << 4) | (bg & 0x0F);
}

void clear_screen(){
	memset((void*)VIDMEM,0,VIDMEM_SIZE);
	term->cursor = 162;
}

int32_t draw_str(char string[], int32_t r, int32_t c){
	uint32_t i=0,j=0,sp=0;
	sp = get_point(r,c);
	for(i=sp;i<(sp+(strlen(string)*2) )-1;i=i+2){
			draw_char(i,string[j++],0,15);
	}
	return PASS_CODE;
}


void write_char(char ascii, int fg, int bg){
	if(ascii == '\b'){
		if(term->cursor%((MAX_COLUMNS+1)*2)>3){
			term->cursor = term->cursor - 2;
			draw_char(term->cursor,' ',fg, bg);
			move_cursor(term->cursor/2);
		}
	}
	else if(ascii == '\r'){	
			while(term->cursor%((MAX_COLUMNS+1)*2)>0){
				term->cursor = term->cursor + 2;
			}
			draw_char(term->cursor,'#',fg,bg);
			draw_char(term->cursor+2,' ',COLOR_BLACK,bg);
			term->cursor +=2;
			move_cursor(term->cursor/2);
	}
	else if(ascii == '\t'){
		for(int i=0;i<8;i++){
			draw_char(term->cursor+2,' ',COLOR_BLACK,bg);
			term->cursor += 2;
		}
		move_cursor(term->cursor/2);
	}
	else if(ascii == '\n'){
		while(term->cursor%((MAX_COLUMNS+1)*2)>0){
			term->cursor = term->cursor + 2;
		}
		move_cursor(term->cursor/2);
		term->cursor += 2;
	}
	else{
		draw_char(term->cursor,ascii,fg,bg);
		draw_char(term->cursor+2,' ',COLOR_BLACK,bg);
		term->cursor += 2;
		move_cursor(term->cursor/2);
	}
}

void write_str(char* string, int fg, int bg){
	int32_t i=0;
	for(i=0;i<strlen(string);i++){
		write_char(string[i], fg, bg);
	}
}

void null_buffer(){
	int c = 0;
	while(c < 100){
		term->buffer[c] = '\0';
		c++;
	}
	term->offset=0;

}

void cover_screen(){
	memset((void*)VIDMEM,1,VIDMEM_SIZE);
}
void vga_init(){
	term->cursor = 162;
	clear_screen();
	draw_char(get_point(1,0), '#', COLOR_BLACK, COLOR_GREEN);
    null_buffer();
	keyboard_install();
}
