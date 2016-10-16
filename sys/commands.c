#include <sys/commands.h>

void exec_cmd(int n, char buff[]){
	
	switch(n){

		case 0:
			clear_screen();
			
			break;
		case 1:
			write_str("\nYou Pressed Exit! System will now halt!");
			
			break;
		case 2:
			write_str("\n");
			cpu_init();
			write_str("\n");
			
			break;
		
		case 3:
			welcome_splash();
			
			break;

		case 4:
			write_str("\nHello, World!\n");
			
			break;
		case 5:
			write_serial(buff);
			break;
		default:
			if(strlen(buff) > 1){
				write_str("\n");
				write_str(buff);
				write_str(": Command not found!");
			}
			
			break;
	}
}
