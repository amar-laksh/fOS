#include <kernel/fos.h>
#include <sys/io_list.h>
unsigned char shift = 0;
unsigned char caps = 0;
unsigned char key_cycle= 0;

struct {
    int16_t mke;
    int16_t bke;
    char buff;
} kbd;
#define cmd  14
char* commands[cmd] = {
    "clear",
    "beep",
    "whoami",
    "cowsay",
    "lspci",
    "serial",
    "tunes",
    "reboot",
    "uptime",
    "settime",
    "lscpu",
    "lsmem",
    "pong",
    "help"
};
console *term;
io_buffer *io_buff;

int process_buffer(){
    char b[2];
    int8_t c=0;
    itoa(term->offset,10,b);
    draw_str("Console offset: ",12,50);
    draw_str(b,13,50);
    char* argv[5];
    int argc = 0;
    int count;
    count = term->offset;
    for (int i = 0; i < count; ++i){
        if(term->buffer[i] == ' ')
            argc++;
    }

    /* If arguments & offset are equal we have all spaces So we skip the command.*/
    if(argc == term->offset){
        null_buffer();
        return;
    }

    argv[0] = str_tok(term->buffer," ");
    for (int i = 1; i < argc+1; i++){
        argv[i] = str_tok(NULL," ");
    }

    for(int i=0; i<cmd;i++){
        if(equals(commands[cmd-1],argv[0]) == 0){
            kprintf("\nThese are the following available commands:\n\n");
            for (int i = 0; i < cmd; ++i)
                kprintf("%s\n",commands[i]);
            c++;
            null_buffer();
        }
        else if(equals(commands[i],argv[0]) == 0){
                exec_cmd(i, argv);
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

void append_buffer(char l){
    if(l == '\b')
        term->buffer
        [(term->offset > 0)?--term->offset:0]
        = 0;
    else if(l !='\r'){
        term->buffer[term->offset++] = l;
    }
}

void keyboard_handler(irq_handler_t handler) {
    unsigned char scancode;
    keyboard_wait();
    scancode = inb(KEY_DEVICE);
    getASCII(scancode);
    irq_ack(KEYBOARD_IRQ);
}


void keyboard_install() {
    keyboard_reset_ps2();
    irq_install_handler(KEYBOARD_IRQ, keyboard_handler);
    sprintf("Initializing PS/2 keyboard driver");
}

void keyboard_reset_ps2() {
    uint8_t tmp = inb(0x61);
    outb(0x61, tmp | 0x80);
    outb(0x61, tmp & 0x7F);
    inb(KEY_DEVICE);
}


void move_cursor(int32_t pos) {
        outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
        outb(FB_DATA_PORT, ((pos >> 8) & 0x00FF));
        outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
        outb(FB_DATA_PORT, pos & 0x00FF);
    }

/*
 * Wait on the keyboard.
 */
void keyboard_wait() {
    while(inb(KEY_PENDING) & 2);
}


void check_for(unsigned char flag, const char table, unsigned char c){
    if(flag == 1){
        kbd.buff = table;
    }

}

/*
 * Add a character to the device buffer.
 */
void getASCII(unsigned char c) {
    int code=0;
    if(c & 0x80){
        if(c == 0xB6|| c== 0xAA)
            shift = 0;
        if(c == 0xBA && caps == 2)
            caps = 1;
        if(c == 0xBA && key_cycle >1){
            caps = 0;
            key_cycle = 0;
        }
    }
    else{
        if(c == 0x36 || c == 0x2A){
            shift = 1;
        }
        else if(c == 0x3A){
            caps = 2;
            key_cycle++;
        }
        else{
            check_for(shift, keytable[c-1].key_shift_value, c);
            check_for(caps, keytable[c-1].key_caps_value, c);
            if(shift == 0 && caps == 0){
                    kbd.buff = keytable[c-1].key_value;
            }
            kbd.mke = keytable[c-1].key_make;
            kbd.bke = keytable[c-1].key_break;
            char l = kbd.buff;
                append_buffer(l);
                if(l=='\r')
                    code = process_buffer();
                if(code == -999)
                    return;
                write_char(l, COLOR_BLACK, COLOR_GREEN);
        }
    }
}
