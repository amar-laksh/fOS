#include <sys/io.h>
#include <sys/io_list.h>
#include <sys/commands.h>
#define cmd  10
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

    /* If arguments & offset are equal we have all spaces
    /* So we skip the command.*/
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
            dprintf("\nThese are the following available commands:\n\n");
            for (int i = 0; i < cmd; ++i)
                dprintf("%s\n",commands[i]);
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

void keyboard_handler(struct regs *r) {
    unsigned char scancode;
    keyboard_wait();
    scancode = inb(KEY_DEVICE);
    irq_ack(KEYBOARD_IRQ);
    getASCII(scancode);
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

/*
 * Add a character to the device buffer.
 */
void getASCII(unsigned char c) {
    if(c & 0x80){
    }
    else{
        int code=0;
        print_registers();
        char l = keytable[c-1].key_value;
        draw_str("f.O.S. - Made By Amar Lakshya",0,20);
        append_buffer(l);
        if(l=='\r')
            code = process_buffer();
        if(code == -999)
            return;
        draw_str("Console Buffer: ",10,50);
        draw_str(term->buffer,11,50);
        write_char(l);
    }

}
