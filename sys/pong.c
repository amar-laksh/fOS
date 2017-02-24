#include<kernel/fos.h>
#define CHECK_AHEAD_CELL(x,y,dx,dy)	get_char_cell(x+dx,y+dy)
int random_seed=0;
pedal left_pedal;
pedal right_pedal;
int game_cycles=0;


int maxrand(int seed,int max){
	random_seed = random_seed+seed * 1103515245 +12345;
	return (unsigned int)(random_seed / 65536) % (max+1);
}

void draw_pedal(int x, int y){
	draw_char(get_point(x-1,y),'I',COLOR_WHITE,COLOR_WHITE);
	draw_char(get_point(x-1,y+1),'I',COLOR_WHITE,COLOR_WHITE);

	draw_char(get_point(x,y),'I',COLOR_WHITE,COLOR_WHITE);
	draw_char(get_point(x,y+1),'I',COLOR_WHITE,COLOR_WHITE);

	draw_char(get_point(x+1,y),'I',COLOR_WHITE,COLOR_WHITE);
	draw_char(get_point(x+1,y+1),'I',COLOR_WHITE,COLOR_WHITE);
}

void erase_pedal(int x, int y){
	draw_char(get_point(x-1,y),' ',COLOR_BLACK,COLOR_BLACK);
	draw_char(get_point(x-1,y+1),' ',COLOR_BLACK,COLOR_BLACK);

	draw_char(get_point(x,y),' ',COLOR_BLACK,COLOR_BLACK);
	draw_char(get_point(x,y+1),' ',COLOR_BLACK,COLOR_BLACK);

	draw_char(get_point(x+1,y),' ',COLOR_BLACK,COLOR_BLACK);
	draw_char(get_point(x+1,y+1),' ',COLOR_BLACK,COLOR_BLACK);
}

void first_push_ball(){
	ball.x = 40;
	ball.y = 10;
	ball.dx = -13;
	ball.dy = -13;
	int seed = maxrand(cycles,123123);
	while( !((ball.dx >= -2 && ball.dx <= 2) && (ball.dy >= -2 && ball.dy <= 2) && (ball.dx != 0)) ){
		ball.dy = maxrand(seed,3);
		ball.dx = maxrand(seed,3);
		if(ball.dx > 2){
			ball.dx = maxrand(seed,1);
			if(ball.dx == 0)
				ball.dx = -1;
			if(ball.dx == 1)
				ball.dx = -2;
		}
		if(ball.dy > 2){
			ball.dy = maxrand(seed,1);
			if(ball.dy == 0)
				ball.dy = -1;
			if(ball.dy == 1)
				ball.dy = -2;
		}
	}
}

void pong_time_handler(irq_handler_t * handler){
	cycles++;
	game_cycles++;
	if(left_pedal.moved == 1){
		draw_pedal(left_pedal.x,left_pedal.y);
		draw_pedal(right_pedal.x,right_pedal.y);
		left_pedal.moved = -13;
	}
	if(game_cycles % 10 == 0){
		if(ball.playing == -13){
			if((ball.y <= 0 || ball.y >= 25)){

				if( (ball.dx <= -1 && ball.dy <= -1)
					|| (ball.dx <= -1 && ball.dy >= 0)
					|| (ball.dx >=0 && ball.dy <=-1)
					|| (ball.dx >= 0 && ball.dy >= 0)){
					ball.dy *= -1;
				}
			}
			else if(CHECK_AHEAD_CELL(ball.y,ball.x,ball.dy,ball.dx) == 'I'){
					if( (ball.dx >= 0 && ball.dy >=0 )
						|| (ball.dx >=0 && ball.dy <= -1))
						ball.dx *= -1;
					else
						ball.dx *= -1;
				}
			draw_char(get_point(ball.y, ball.x), ' ', COLOR_BLACK, COLOR_BLACK);
			ball.x += ball.dx;
			ball.y += ball.dy;
			draw_char(get_point(ball.y, ball.x), '0', COLOR_RED, COLOR_RED);
		}
	}
	irq_ack(TIMER_IRQ);
}

void pong_keyboard_handler(irq_handler_t * handler){
	unsigned char scancode;
	keyboard_wait();
	scancode = inb(KEY_DEVICE);
	irq_ack(KEYBOARD_IRQ);
	if(scancode == 0x48){
		left_pedal.moved = 1;
		erase_pedal(left_pedal.x,left_pedal.y);
		erase_pedal(right_pedal.x,right_pedal.y);
		left_pedal.x -= 1;
		right_pedal.x -= 1;
	}
	if(scancode == 0x50){
		left_pedal.moved = 1;
		erase_pedal(left_pedal.x,left_pedal.y);
		erase_pedal(right_pedal.x,right_pedal.y);
		left_pedal.x += 1;
		right_pedal.x += 1;
	}
	if(scancode == 0x10){
		irq_uninstall_handler(KEYBOARD_IRQ);
		irq_install_handler(KEYBOARD_IRQ, keyboard_handler);
		irq_uninstall_handler(TIMER_IRQ);
		irq_install_handler(TIMER_IRQ, timer_handler);
		return;
	}
}

void play_pong(){
	irq_uninstall_handler(TIMER_IRQ);
	irq_install_handler(TIMER_IRQ, pong_time_handler);
	clear_screen();
	left_pedal.moved = right_pedal.moved = -13;
	left_pedal.x = right_pedal.x = 10;
	left_pedal.y = 1;
	right_pedal.y = 78;
	first_push_ball();
	if(ball.dx == -2)
		ball.dx = -1;
	if(ball.dx == 2)
		ball.dx = 1;
	if(ball.dy == -2)
		ball.dy = -1;
	if(ball.dy == 2)
		ball.dy = 1;
	ball.playing = -13;
	irq_uninstall_handler(KEYBOARD_IRQ);
	irq_install_handler(KEYBOARD_IRQ, pong_keyboard_handler);
}