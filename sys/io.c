#include <sys/io.h>

#define FB_COMMAND_PORT 0x3D4
#define FB_DATA_PORT 0x3D5
#define FB_HIGH_BYTE_COMMAND 14
#define FB_LOW_BYTE_COMMAND 15
#define PIC1_PORT_A 0x20
#define PIC2_PORT_A 0xA0
#define PIC1_START_INTERRUPT 0x20
#define PIC2_START_INTERRUPT 0x28
#define PIC2_END_INTERRUPT PIC2_START_INTERRUPT + 7
#define PIC_ACK 0x20
#define KBD_DATA_PORT 0x60
#define PORT 0x40   /* COM1 */


void kbd_set_leds (bool num, bool caps, bool scroll) {
    uint8_t data = 0;
    data = (scroll) ? (data | 1) : (data & 1);
    data = (num) ? (num | 2) : (num & 2);
    data = (caps) ? (num | 4) : (num & 4);
    outb(0x06,0xED);
    outb(0x06,data);
}

void pic_acknowledge(uint32_t interrupt) {
	if (interrupt < PIC1_START_INTERRUPT || 
			interrupt > PIC2_END_INTERRUPT)
		return;

	if (interrupt < PIC2_START_INTERRUPT) {
		outb(PIC1_PORT_A, PIC_ACK);
	} else {
		outb(PIC2_PORT_A, PIC_ACK);
	}
}

int8_t readb(int8_t port) {
	return inb(port);
}

int8_t read_scan_code() {
	return inb(KBD_DATA_PORT);
}

void move_cursor(int32_t pos) {
        outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
        outb(FB_DATA_PORT, ((pos >> 8) & 0x00FF));
	outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
        outb(FB_DATA_PORT, pos & 0x00FF);
    }

int8_t get_kbd() {
        int8_t sc;
	while(read_scan_code()){
                sc = '\0';
                sc = read_scan_code();
        if(sc == '\x1C'){
                        while(read_scan_code() != '\x9C'){

                        }
            return '\r';
        }
		if(sc == '\x0E'){
                        while(read_scan_code() != '\x8E'){

                        }
			return '\b';
		}
                if(sc == '\x1E'){
                        while(read_scan_code() != '\x9E'){

                        }
			return 'a';
		}
                if(sc == '\x30'){
                        while(read_scan_code() != '\xB0'){

                        }
			return 'b';
		}
                if(sc == '\x2E'){
                        while(read_scan_code() != '\xAE'){

                        }
			return 'c';
		}
                if(sc == '\x20'){
                        while(read_scan_code() != '\xA0'){

                        }
			return 'd';
		}
                if(sc == '\x12'){
                        while(read_scan_code() != '\x92'){

                        }
			return 'e';
		}
                if(sc == '\x21'){
                        while(read_scan_code() != '\xA1'){

                        }
			return 'f';
		}
                if(sc == '\x22'){
                        while(read_scan_code() != '\xA2'){

                        }
			return 'g';
		}
                if(sc == '\x23'){
                        while(read_scan_code() != '\xA3'){

                        }
			return 'h';
		}
                if(sc == '\x17'){
                        while(read_scan_code() != '\x97'){

                        }
			return 'i';
		}
                if(sc == '\x24'){
                        while(read_scan_code() != '\xA4'){

                        }
			return 'j';
		}
                if(sc == '\x25'){
                        while(read_scan_code() != '\xA5'){

                        }
			return 'k';
		}
                if(sc == '\x26'){
                        while(read_scan_code() != '\xA6'){

                        }
			return 'l';
		}
                if(sc == '\x32'){
                        while(read_scan_code() != '\xB2'){

                        }
			return 'm';
		}
                if(sc == '\x31'){
                        while(read_scan_code() != '\xB1'){

                        }
			return 'n';
		}
                if(sc == '\x18'){
                        while(read_scan_code() != '\x98'){

                        }
			return 'o';
		}
                if(sc == '\x19'){
                        while(read_scan_code() != '\x99'){

                        }
			return 'p';
		}
                if(sc == '\x10'){
                        while(read_scan_code() != '\x90'){

                        }
			return 'q';
		}
                if(sc == '\x13'){
                        while(read_scan_code() != '\x93'){

                        }
			return 'r';
		}
                if(sc == '\x1F'){
                        while(read_scan_code() != '\x9F'){

                        }
			return 's';
		}
                if(sc == '\x14'){
                        while(read_scan_code() != '\x94'){

                        }
			return 't';
		}
                if(sc == '\x16'){
                        while(read_scan_code() != '\x96'){

                        }
			return 'u';
		}
                if(sc == '\x2F'){
                        while(read_scan_code() != '\xAF'){

                        }
			return 'v';
		}

                if(sc == '\x11'){
                        while(read_scan_code() != '\x91'){

                        }
			return 'w';
		}

                if(sc == '\x2D'){
                        while(read_scan_code() != '\xAD'){

                        }
			return 'x';
		}

                if(sc == '\x15'){
                        while(read_scan_code() != '\x95'){

                        }
			return 'y';
		}
                if(sc == '\x2C'){
                        while(read_scan_code() != '\xAC'){

                        }
			return 'z';
		}
                if(sc == '\x39'){
                        while(read_scan_code() != '\xB9'){

                        }
			return ' ';
		}
		if(sc == '\x0B'){
                        while(read_scan_code() != '\x8B'){

                        }
                        return '0';
                }
		if(sc == '\x02'){
                        while(read_scan_code() != '\x82'){

                        }
                        return '1';
                }
		if(sc == '\x03'){
                        while(read_scan_code() != '\x83'){

                        }
                        return '2';
                }
		if(sc == '\x04'){
                        while(read_scan_code() != '\x84'){

                        }
                        return '3';
                }
		if(sc == '\x05'){
                        while(read_scan_code() != '\x85'){

                        }
                        return '4';
                }
		if(sc == '\x06'){
                        while(read_scan_code() != '\x86'){

                        }
                        return '5';
                }
		if(sc == '\x07'){
                        while(read_scan_code() != '\x87'){

                        }
                        return '6';
                }
		if(sc == '\x08'){
                        while(read_scan_code() != '\x88'){

                        }
                        return '7';
                }
		if(sc == '\x09'){
                        while(read_scan_code() != '\x89'){

                        }
                        return '8';
                }
		if(sc == '\x0A'){
                        while(read_scan_code() != '\x8A'){

                        }
                        return '9';
                }

	}
	return '\0';
}


