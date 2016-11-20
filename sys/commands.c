#include <sys/commands.h>

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

void some_tune(){
	char notes[12] = {
		'G','G','A','G','C','B'
		,'G','G','A','G','D','C'
	};
	for (int i = 0; i < 12; ++i)
	{
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

void exec_cmd(int n, char buff[]){
	char* argv[5];
    int argc = 0;
	int count;
    count = strlen(buff);
	switch(n){

		case 0:
			clear_screen();
			term->cursor =2;
			break;
		case 1:
		    for (int i = 0; i < count; ++i)
		    {
		        if(buff[i] == ' ')
		            argc++;
		    }
		    argv[0] = str_tok(buff," ");
		    for (int i = 1; i < argc+1; i++)
		    {
		        argv[i] = str_tok(NULL," ");
		    }
			dprintf("\nHere's a beep at: %dHz with delay of: %d\n",atoi(argv[1]),atoi(argv[2]));
			beep_it(atoi(argv[1]), atoi(argv[2]));
			break;
		case 2:
			dprintf("\nWell my name is:\n %s",cpu.processor_name);
			break;
		case 3:
			cowsay(buff);
			break;
		case 4:
		    for (int i = 0; i < count; ++i)
		    {
		        if(buff[i] == ' ')
		            argc++;
		    }
		    argv[0] = str_tok(buff," ");
		    for (int i = 1; i < argc+1; i++)
		    {
		        argv[i] = str_tok(NULL," ");
		    }
			//mm_print_out();
			pci_proc_dump(atoi(argv[1]));
			break;
		case 5:
			write_serial("\n");
			write_serial(buff);
			write_serial("\n");
			break;
		case 6:
			note_test();
			delay(10000);
			some_tune();
			delay(10000);
			marry_song();
			delay(10000);
			splash();
			break;
		default:
			if(strlen(buff) > 1){
				kprintf("\n%s: not found!\n",buff);
			}
			
			break;
	}
}
