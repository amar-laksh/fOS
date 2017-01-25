#include <kernel/fos.h>
#define CHECK_FOR(flag ,table) if(flag == 1){kbd.buff = table;};

unsigned int shift_v = 0;
unsigned int caps_v = 0;
unsigned int key_cycle_v= 0;
unsigned int p=642;
unsigned int vim_cycles = 0;

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
	clear_screen();
	irq_uninstall_handler(KEYBOARD_IRQ);
	irq_install_handler(KEYBOARD_IRQ, vim_keyboard_handler);
	clear_screen();
}


void getWords   (
				unsigned char c
				)
{
	int code=0;
	if(c & 0x80){
		if(c == 0xB6|| c== 0xAA)
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
				irq_install_handler(KEYBOARD_IRQ, keyboard_handler);
				irq_uninstall_handler(TIMER_IRQ);
				irq_install_handler(TIMER_IRQ, timer_handler);
			}
			draw_char(p, l, COLOR_BLACK, COLOR_WHITE);
			p+=2;
			draw_char(p+2, ' ', COLOR_BLACK, COLOR_WHITE);
			move_cursor((p/2));
		}
	}
}
