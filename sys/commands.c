#include <sys/commands.h>
#include <sys/cpu.h>



/* TODO- The case 3 is temperory. Create a text-based graphics engine.*/
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
	dprintf("\n");
	int length = strlen(buff);
	for(int i=1;i<length;i++){
		dprintf("-");
	}
	dprintf("\n<  %s  >\n",buff);
	for(int i=1;i<length;i++){
		dprintf("-");
	}
	dprintf("\n");

	for (int i = 0; i <length; i++){
		dprintf(" ");
	}
	dprintf("\\    ^___^\n");
	for (int i = 0; i <length+1; i++){
		dprintf(" ");
	}
	dprintf("\\  (o o) \\________\n");
	for (int i = 0; i <length+1; i++){
		dprintf(" ");
	}
	dprintf("   (___)\\         )\\/\\\n");
	for (int i = 0; i <length+1; i++){
		dprintf(" ");
	}
	dprintf("         ||----w |\n");
	for (int i = 0; i <length+1; i++){
		dprintf(" ");
	}
	dprintf("         ||     ||");

}
void exec_cmd(int n, char buff[]){
	switch(n){

		case 0:
			clear_screen();
			term->cursor =2;
			break;
		case 1:
			write_str("\nYou Pressed Exit! System will now halt!\n");
			timer_wait(10);
			write_str("Just kidding, can't perform that craziness right now :( ");
			break;
		case 2:
			dprintf("\nWell my name is:\n %s",cpu.processor_name);
			break;
		case 3:
			cowsay(buff);
			break;
		case 4:
			dprintf("\n");
			//mm_print_out();
			pci_proc_dump();
			break;
		case 5:
			write_serial("\n");
			write_serial(buff);
			write_serial("\n");
			break;
		default:
			if(strlen(buff) > 1){
				kprintf("\n%s: not found!\n",buff);
			}
			
			break;
	}
}
