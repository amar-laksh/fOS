/* TODO- The case 3 is temperory. Create a text-based graphics engine.*/
/* TODO - Create a proper scrolling mechanism */
#include <kernel/fos.h>
#define KBRD_INTRFC 0x64

/* keyboard interface bits */
#define KBRD_BIT_KDATA 0 
#define KBRD_BIT_UDATA 1 
 
#define KBRD_IO 0x60 
#define KBRD_RESET 0xFE 
 
#define bit(n) (1<<(n))
#define GetInInterrupt(arg)  __asm__ __volatile__ \
							("int %0"::"N"(arg));

 
/* Check if bit n in flags is set */
#define check_flag(flags, n) ((flags) & bit(n))

void read_text	(
				char* ptr
				, unsigned int count
				)
{
	clear_screen();
		unsigned int j=0;
		term.cursor = 0;
		while(j < (count)){
		  if(ptr[j] == 0x5c && ptr[j+1] == 0x6e){
			ptr[j] = '\n';
			ptr[j+1] = 0x0;
		  }
		  if(ptr[j] == 0x5c && ptr[j+1] == 0x74){
			ptr[j] = '\t';
			ptr[j+1] = 0x0;
		  }

			write_char(ptr[j],COLOR_BLACK, COLOR_WHITE);
		  j++;
		}
}

void create(
			char* args
			,int x
			, int y
			)
{
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

void cowsay(
			char buff[]
			)
{
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
	for (int i = 0; i < 7; ++i)
	{
		play_note(notes[i],6,4);
	}
	for (int i = 6; i >= 0 ; --i)
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
	for (int i = 0; i < 26; ++i){
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

void set_clock	(
				int h
				, int m
				, int s
				)
{
	seconds = s;
	minutes = m;
	hours = h;
	cycles = 0;
	cycles = ( (s*100) + (m*6000) + (h*360000) );
	return;
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
	if(cpu.extended_cpu_features[3] & (1<<29)){
		kprintf("Architecture:\t\tx86_64\n");
		kprintf("CPU op-mode(s):\t      32-bit, 64-bit\n");
		kprintf("Byte order:\t\t  Little-Endian\n");
	}
	else{
		kprintf("Architecture:\t\tx86\n");
		kprintf("CPU op-mode(s):\t      32-bit\n");
		kprintf("Byte order:\t\t  Little-Endian\n");
	}
	kprintf("Vendor ID\t\t    %s\n"
				,cpu.cpu_vendor_string);
	kprintf("Model Name\t\t   %s\n"
				,cpu.processor_name);
	kprintf("L1i Cache:\t\t   %dK\n"
				, cpu.det_cache_params.cache_total_size[0] >> 10);
	kprintf("L1d Cache:\t\t   %dK\n"
				, cpu.det_cache_params.cache_total_size[1] >> 10);
	kprintf("L2 Cache:\t\t    %dK\n"
				, cpu.det_cache_params.cache_total_size[2] >> 10);
	kprintf("L3 Cache:\t\t    %dK\n"
				, cpu.det_cache_params.cache_total_size[3] >> 10);
	kprintf("CPU FLAGS:\t\t  ");
	int c=0;
	for (int i = 0; i < 64; ++i){
		if(	cpu.cpu_flags[i].flag == 1 
				&&  
			cpu.cpu_flags[i].flag != 2){
			kprintf("%s|",cpu.cpu_flags[i].name);
			c++;
		}
	}
	kprintf("\nTotal CPU FLags: %d\n", c);
}

void *memset_h	(
				void *dest
				, int16_t val
				, size_t count
				)
{
	int16_t *temp = (int16_t *)dest;
	for( ; count != 0; count--) *temp++ = val;
	return dest;
}

void scroll(
			int flag
			)
{
	char* fb = (char*) VIDMEM;
		if  (
			vga_fb.array_offset != VIDMEM_SIZE 
						&&
			vga_fb.vga_offset != 0
			)
		{
			vga_fb.array_offset = VIDMEM_SIZE;
			vga_fb.vga_offset = 0;
		}
		int i=0;
		if(flag == 1){

		}
		else{
		}
}

void resume(){
	vga_fb.vga_buffer = malloc(VIDMEM_SIZE*2);
	memset(vga_fb.vga_buffer, 0, VIDMEM_SIZE*2);
	sprintf("\n");
	mm_print_out();
	char* ptr = (char*)memory_t.modules[1].module_start;
	read_text(ptr,(memory_t.modules[0].module_end - memory_t.modules[0].module_start));
	here:
	if(inb(KEY_DEVICE) == 0x24)
		scroll(1);
	if(inb(KEY_DEVICE) == 0x25)
		scroll(0);
	if(inb(KEY_DEVICE) == 0x10){
		clear_screen();
		return;
	}
	goto here;
}

void asm_execute(
				char* buff[5]
				)
{
	if(equals(buff[1],"int") == 0){
		switch(atoi(buff[2])){
			case 0:
				GetInInterrupt(0);
				break;
			case 1:
				GetInInterrupt(1);
				break;
			case 2:
				GetInInterrupt(2);
				break;
			case 3:
				GetInInterrupt(4);
				break;
			case 4:
				GetInInterrupt(4);
				break;
			case 5:
				GetInInterrupt(5);
				break;
		}
	}
	kprintf("\n%s, %d\n", buff[1], atoi(buff[2]));

}
void exec_cmd	(
				int n
				, char* buff[5]
				)
{
	switch(n){
		case 0:
			clear_screen();
			term.cursor = 2;
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
			delay(200/441);
			some_tune();
			delay(200/441);
			marry_song();
			delay(200/441);
			splash();
			break;
		case 7:
			reboot();
			break;
		case 8:
			kprintf("\nThe system has been up for %d cycles.\n"
						, cycles);
			break;
		case 9:
			set_clock	(
						atoi(buff[1])
						, atoi(buff[2])
						, atoi(buff[3])
						);
			break;
		case 10:
			kprintf("\nThe summary about the CPU: \n");
			lscpu();
			break;
		case 11:
			lsmem();
			break;
		case 12:
			play_pong();
			break;
		case 13:
			resume();
			break;
		case 14:
			asm_execute(buff);
			break;
		case 15:
			vim();
			break;
		default:
			if(strlen((const char*)buff) > 1){
				kprintf("\n%s: not found!\n",buff);
			}
			
			break;
	}
	term.color_bg_value = COLOR_BLACK;
	term.color_fg_value = COLOR_GREEN;
}
