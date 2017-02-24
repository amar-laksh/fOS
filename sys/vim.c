#include <kernel/fos.h>
#define CHECK_FOR(flag ,table) if(flag == 1){kbd.buff = table;};

unsigned int shift_v = 0;
unsigned int caps_v = 0;
unsigned int key_cycle_v= 0;
unsigned int p=642;
unsigned int vim_cycles = 0;
unsigned int column = 0;
char* buff;

void vim_keyboard_handler	(
							irq_handler_t* handler
							)
{
	unsigned char scancode;
	keyboard_wait();
	scancode = inb(KEY_DEVICE);
	getWords(scancode);
	irq_ack(KEYBOARD_IRQ);
}

void vim()
{
	p = 642;
	irq_uninstall_handler(KEYBOARD_IRQ);
	irq_install_handler(KEYBOARD_IRQ, vim_keyboard_handler);
	clear_screen();
	term.color_fg_value = COLOR_WHITE;
	term.color_bg_value = COLOR_DARK_GREY;
	paint_screen();
	here:
		goto here;
}


void getWords   (
				unsigned char c
				)
{
	int code=0;
	if(c & 0x80){
		if(c == 0xB6|| c == 0xAA)
			shift_v = 0;
		if(c == 0xBA && caps_v == 2)
			caps_v = 1;
		if(c == 0xBA && key_cycle_v >1){
			caps_v = 0;
			key_cycle_v = 0;
		}
	}
	else{
		if(c == 0x36 || c == 0x2A){
			shift_v = 1;
		}
		else if(c == 0x3A){
			caps_v = 2;
			key_cycle_v++;
		}
		else{
			CHECK_FOR(shift_v, keytable[c-1].key_shift_value);
			CHECK_FOR(caps_v, keytable[c-1].key_caps_value);
			if(shift_v == 0 && caps_v == 0){
					kbd.buff = keytable[c-1].key_value;
			}
			char l = kbd.buff;
			if(l == ':'){
				irq_uninstall_handler(KEYBOARD_IRQ);
				term.color_bg_value = COLOR_BLACK;
				term.color_fg_value = COLOR_GREEN;
				irq_install_handler(KEYBOARD_IRQ, keyboard_handler);
			}
			draw_char(p, l, term.color_bg_value, term.color_fg_value);
			draw_char(p+2, ' ', term.color_bg_value, term.color_fg_value);
			column = (p/2)%80;
			itoa(column, 10, buff);
			draw_str(":",24,4);
			draw_str(buff, 24, 1);
			move_cursor((p/2)+1);
			p+=2;
		}
	}
}
