/* TODO- The case 3 is temperory. Create a text-based graphics engine.*/
#include <kernel/fos.h>
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

void cpu_t_set(){
	if(cpu.extended_cpu_features[3] & (1<<29))
		cpu_t.arc = "x86_64";
	else
		cpu_t.arc = "x86";
	
	if(equals(cpu_t.arc, "x86_64") == 0 
		|| equals(cpu_t.arc, "x86") == 0){
			if(equals(cpu_t.arc, "x86_64") == 0)
				cpu_t.op_modes = "32-bit, 64-bit";
			else
				cpu_t.op_modes = "32-bit";
			cpu_t.byte_order = "Little Endian";
	}
}

void lsmem(){
	kprintf("\n");
	kprintf("Total Memory:%d MB\n", memory_t.total_mem);
	kprintf("Used Memory:%d Kb\n", memory_t.used_mem);
	kprintf("Un-Used Memory:%d Kb\n", memory_t.unused_mem);
	kprintf("Kernel Start at:0x%x\n", memory_t.kernel_start);
	kprintf("Kernel End at:0x%x\n", memory_t.kernel_end);
}
void lscpu(){
	cpu_t_set();
	kprintf("\n");
	kprintf("Architecture:\t\t%s\n", cpu_t.arc);
	kprintf("CPU op-mode(s):\t\t%s\n", cpu_t.op_modes);
	kprintf("Byte order:\t\t%s\n", cpu_t.byte_order);
	kprintf("CPU(s):\t\t%s\n", cpu_t.cpus);
	kprintf("On-line CPU(s) list:\t\t%s\n", cpu_t.online_cpu_list);
	kprintf("Thread(s) per core:\t\t%s\n", cpu_t.threads_per_core);
	kprintf("Core(s) per socket:\t\t%s\n", cpu_t.cores_per_socket);
	kprintf("Socket(s):\t\t%s\n", cpu_t.sockets);
	kprintf("NUMA node(s):\t\t%s\n", cpu_t.NUMA_nodes);
	kprintf("Vendor ID:\t\t%s\n", cpu_t.vendor_id);
	kprintf("CPU Family:\t\t%s\n", cpu_t.cpu_family);
	kprintf("Model:\t\t%s\n", cpu_t.model);
	kprintf("Model Name:\t\t%s\n", cpu_t.model_name);

	kprintf("CPU FEATURES:\n");
	for (int i = 0; i < 52; ++i){
		if(cpu.cpu_features[i].flag == 1)
		kprintf("%s|",cpu.cpu_features[i].name);
	}

	kprintf("\n\nCPU INSTRUCTIONS:\n");
	for (int i = 0; i < 23; ++i){
		if(cpu.cpu_instructions[i].flag == 1)
		kprintf("%s|",cpu.cpu_instructions[i].name);
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
		case 11:
			lsmem();
			break;
		default:
			if(strlen((const char*)buff) > 1){
				kprintf("\n%s: not found!\n",buff);
			}
			
			break;
	}
}
