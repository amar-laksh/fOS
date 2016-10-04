#include <sys/commands.h>

void exec_cmd(int n, char buff[]){
	
	switch(n){

		case 0:
			clear_screen();
			null_buffer();
			break;
		case 1:
			write_str("\nYou Pressed Exit! System will now halt!");
			null_buffer();
			break;
		case 2:
			write_str("\n");
			cpu_init();
			write_str("\n");
			null_buffer();
			break;
		
		case 3:
			welcome_splash();
			null_buffer();
			break;

		case 4:
			write_str("\nHello, World!\n");
			null_buffer();
			break;
		
		default:
			if(strlen(buff) > 1){
				write_str("\n");
				write_str(buff);
				write_str(": Command not found! (baap ki shaadi h? *_* )");
			}
			null_buffer();
			break;
	}
}
