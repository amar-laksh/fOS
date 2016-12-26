// TODO - ADD MOUSE FUNCTIONALITY

#include <kernel/fos.h>

unsigned char mouse_cycle=0;     //unsigned char
signed char mouse_byte[3];    //signed char
signed char mouse_x=0;         //signed char
signed char mouse_y=0;         //signed char
signed char old_mouse_x=1;         //signed char
signed char old_mouse_y=1;         //signed char
int turn=0;

inline void mouse_wait(unsigned char a_type) //unsigned char
{
  unsigned int _time_out=100000; //unsigned int
  if(a_type==0)
  {
    while(_time_out--) //Data
    {
      if((inb(0x64) & 1)==1)
      {
        return;
      }
    }
    return;
  }
  else
  {
    while(_time_out--) //Signal
    {
      if((inb(0x64) & 2)==0)
      {
        return;
      }
    }
    return;
  }
}

inline void mouse_write(unsigned char a_write) //unsigned char
{
  //Wait to be able to send a command
  mouse_wait(1);
  //Tell the mouse we are sending a command
  outb(0x64, 0xD4);
  //Wait for the final part
  mouse_wait(1);
  //Finally write
  outb(0x60, a_write);
}

unsigned char mouse_read()
{
  //Get's response from mouse
  mouse_wait(0);
  return inb(0x60);
}
//Mouse functions
void mouse_handler(struct regs *a_r) //struct regs *a_r (not used but just there)
{
  switch(mouse_cycle)
  {
    case 0:
      mouse_byte[0]=inb(0x60);
      if( (mouse_byte[0] & 0x07) > 0){
        term->cursor_value = ".";
      }
      else{
        term->cursor_value = " ";
      }
      mouse_cycle++;
      turn++;
      break;
    case 1:
      mouse_byte[1]=inb(0x60);
      mouse_x=mouse_byte[1];
      mouse_cycle++;
      turn++;
      break;
    case 2:
      mouse_byte[2]=inb(0x60);
      mouse_y=mouse_byte[2];
      mouse_cycle=0;
      turn++;
      break;
  }
  if(1){
        if(mouse_cycle == 0 && turn == 9){
          if(mouse_x == 0 && mouse_y < 0 && old_mouse_y != mouse_y){
            term->cursor += 160;
            old_mouse_y = mouse_y;
            move_cursor(term->cursor/2);
          }
          else if(mouse_x == 0 && mouse_y >=0 && old_mouse_y != mouse_y){
            term->cursor -= 160;
            old_mouse_y = mouse_y;
            move_cursor(term->cursor/2);
          }
          else if(mouse_y == 0 && mouse_x < 0 && old_mouse_x != mouse_x){
            term->cursor -= 2;
            old_mouse_x = mouse_x;
            move_cursor(term->cursor/2);
          }
          else if(mouse_y == 0 && mouse_x >= 0 && old_mouse_x != mouse_x){
            term->cursor += 2;
            old_mouse_x = mouse_x;
            move_cursor(term->cursor/2);
          }
          turn = 0;
      }
  }

  draw_char(term->cursor, term->cursor_value[0],COLOR_BLACK, COLOR_GREEN);
  irq_ack(12);
}


void mouse_install()
{
  unsigned char _status;  //unsigned char

  //Enable the auxiliary mouse device
  mouse_wait(1);
  outb(0x64, 0xA8);
 
  //Enable the interrupts
  mouse_wait(1);
  outb(0x64, 0x20);
  mouse_wait(0);
  _status=(inb(0x60) | 2);
  mouse_wait(1);
  outb(0x64, 0x60);
  mouse_wait(1);
  outb(0x60, _status);
 
  //Tell the mouse to use default settings
  mouse_write(0xF6);
  mouse_read();  //Acknowledge
 
  //Enable the mouse
  mouse_write(0xF4);
  mouse_read();  //Acknowledge

  //Setup the mouse handler
  irq_install_handler(12, mouse_handler);
}