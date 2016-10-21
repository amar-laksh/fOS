#include <sys/commands.h>

CPU_TOPOLOGY cpu;


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

void start_game(){
			clear_screen();
			int x = 10, y = 10;
			create("player",x,y);
			while(read_scan_code()){
				char l = get_kbd();
				if(l == 'w'){
					create("nothing",x,y);
					create("player",x,y-=1);
					
				}
				else if(l == 'a'){
					create("nothing",x,y);
					create("player",x-=2,y);
					
				}
				else if(l == 'd'){
					create("nothing",x,y);
					create("player",x+=2,y);
					
				}
				else if(l == 's'){
					create("nothing",x,y);
					create("player",x,y+=1);
					
				}
				else if(l == 'q'){
					break;
				}
				if(l == '\r'){
					while(read_scan_code()){
					char w = get_kbd();
						if(w == '\b')
							break;
						int b = y - 1;
						draw_char((b*160)+(x*2),w,COLOR_BLACK,COLOR_RED);
					}
				}
			}
}

void exec_cmd(int n, char buff[]){
	switch(n){

		case 0:
			clear_screen();
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
			start_game();
			break;
		case 4:
			write_str("\nHello, World!\n");
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
