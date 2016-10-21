#include <sys/commands.h>


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
			write_str("\n");
			write_str("I am root");
			write_str("\n");
			break;
		case 3:
			welcome_splash();
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
				write_str("\n");
				write_str(buff);
				write_str(": Command not found!");
			}
			
			break;
	}
}
