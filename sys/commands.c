/* TODO- The case 3 is temperory. Create a text-based graphics engine.*/
#include <sys/commands.h>
#define KBRD_INTRFC 0x64
 
/* keyboard interface bits */
#define KBRD_BIT_KDATA 0 
#define KBRD_BIT_UDATA 1 
 
#define KBRD_IO 0x60 
#define KBRD_RESET 0xFE 
 
#define bit(n) (1<<(n)) 
 
/* Check if bit n in flags is set */
#define check_flag(flags, n) ((flags) & bit(n))


void create(char* args,int x, int y){
	if(equals(args,"player") == 0){
		draw_char(get_point(y,x-1),'=',0,COLOR_RED);
		draw_char(get_point(y,x),'#',0,COLOR_RED);
		draw_char(get_point(y,x+1),'=',0,COLOR_RED);
	}
	else if(equals(args,"nothing") == 0){
		draw_char(get_point(y,x-1),' ',0,COLOR_BLACK);
		draw_char(get_point(y,x),' ',0,COLOR_BLACK);
		draw_char(get_point(y,x+1),' ',0,COLOR_BLACK);	
	}
}

void cowsay(char buff[]){
	kprintf("\n");
	int length = strlen(buff);
	for(int i=1;i<length;i+=2){
		kprintf("-");
	}
	kprintf("\n<  %s  >\n",buff);
	for(int i=1;i<length+3;i+=2){
		kprintf("-");
	}
	kprintf("\n");

	for (int i = 0; i <length; i++){
		kprintf(" ");
	}
	kprintf("\\    ^___^\n");
	for (int i = 0; i <length+1; i++){
		kprintf(" ");
	}
	kprintf("\\  (o o) \\________\n");
	for (int i = 0; i <length+1; i++){
		kprintf(" ");
	}
	kprintf("   (___)\\         )\\/\\\n");
	for (int i = 0; i <length+1; i++){
		kprintf(" ");
	}
	kprintf("         ||----w |\n");
	for (int i = 0; i <length+1; i++){
		kprintf(" ");
	}
	kprintf("         ||     ||");

}

void some_tune(){
	char notes[12] = {
		'G','G','A','G','C','B'
		,'G','G','A','G','D','C'
	};
	for (int i = 0; i < 12; ++i){
		play_note(notes[i],6,1);
	}
}

void splash(){
		char notes[7] = {
		'A','B','C','D','E','F','G'
	};
	for (int i = 0; i < 8; ++i)
	{
		play_note(notes[i],6,4);
	}
	for (int i = 7; i >= 0 ; --i)
	{
		play_note(notes[i],6,4);
	}
}
void note_test(){
	for (int i = 1; i < 13; ++i){
		play_note('C',i,1);
	}
}

void marry_song(){
	char notes[26] = {'E','D','C','D','E','E','E'
					,'D','D','D','E','E','E'
					,'E','D','C','D','E','E','E'
					,'E','D','D','E','D','C'
					};
	for (int i = 0; i < 27; ++i){
		play_note(notes[i],6,1);
	}
}


void reboot(){
    uint8_t temp;
    asm volatile ("cli"); 
    do{
        temp = inb(KBRD_INTRFC);
        if (check_flag(temp, KBRD_BIT_KDATA) != 0)
            inb(KBRD_IO); 
    } while (check_flag(temp, KBRD_BIT_UDATA) != 0);
    outb(KBRD_INTRFC, KBRD_RESET); 
loop:
    asm volatile ("hlt");
    goto loop; 
}

void set_clock(int h, int m, int s){
	seconds = s;
	minutes = m;
	hours = h;
	cycles = 0;
	cycles = ( (s*100) + (m*6000) + (h*360000) );
	return;
}

void lscpu(){
	kprintf("\nPrinting CPU misc info:\n");
	for (int i = 0; i < 4; ++i){
		kprintf("%d\t",cpu.cpu_misc_info[i]);
	}
	kprintf("\nPrinting Processor signature:\n");
	for (int i = 0; i < 5; ++i){
		kprintf("%d\t",cpu.processor_signature[i]);
	}
}
void exec_cmd(int n, char* buff[5]){
	switch(n){
		case 0:
			clear_screen();
			term->cursor = 2;
			break;
		case 1:
			kprintf("\nHere's a beep at: %dHz with delay of: %d\n"
									,atoi(buff[1]),atoi(buff[2]));
			beep_it(atoi(buff[1]), atoi(buff[2]));
			break;
		case 2:
			kprintf("\nWell my name is:\n %s",cpu.processor_name);
			break;
		case 3:
			cowsay(buff[1]);
			break;
		case 4:
			pci_proc_dump(atoi(buff[1]));
			break;
		case 5:
		    sprintf("\n%s\n",buff[1]);
			break;
		case 6:
			note_test();
			delay(200);
			some_tune();
			delay(200);
			marry_song();
			delay(200);
			splash();
			break;
		case 7:
			reboot();
			break;
		case 8:
			kprintf("\nThe system has been up for %d cycles.\n", cycles);
			break;
		case 9:
			set_clock(atoi(buff[1]), atoi(buff[2]), atoi(buff[3]));
			break;
		case 10:
			lscpu();
			break;
		default:
			if(strlen((const char*)buff) > 1){
				kprintf("\n%s: not found!\n",buff);
			}
			
			break;
	}
}
