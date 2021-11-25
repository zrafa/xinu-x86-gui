/* source: https://forum.osdev.org/viewtopic.php?t=10247 */

#include <xinu.h>
#include <stdio.h>
#include <string.h>

//#define inp(p)          (*(port unsigned char *)(p))
#define inportb(p)      inb(p)
//#define outp(p,v)       (*(port unsigned char *)(p) = (v))
#define outportb(p,v)   outb(p,v)

#define WIDHT 1024
#define HEIGHT 1024


//Mouse.inc by SANiK
//License: Use as you wish, except to cause damage
byte mouse_cycle=0;     //unsigned char
unsigned char mouse_byte[3];    //signed char
int mouse_x=0;         //signed char
int mouse_y=0;         //signed char

//Mouse functions
void mouse_handler(struct regs *a_r) //struct regs *a_r (not used but just there)
{
	uint8 d, state;
	int i, rel_x, rel_y;
	mouse_cycle=0;
  for (i=0; i<3; i++)
  switch(mouse_cycle)
  {
    case 0:
      mouse_byte[0]=inportb(0x60);
      mouse_cycle++;
      break;
    case 1:
      mouse_byte[1]=inportb(0x60);
      mouse_cycle++;
      break;
    case 2:
      mouse_byte[2]=inportb(0x60);
      // mouse_x=mouse_byte[1];
      // mouse_y=mouse_byte[2];
      mouse_cycle=0;

	state = mouse_byte[0];
	d = mouse_byte[1];
	rel_x = d - ((state << 4) & 0x100);
	d = mouse_byte[2];
	rel_y = d - ((state << 3) & 0x100);
	mouse_x = mouse_x + rel_x;
	mouse_y = mouse_y + (-1)*rel_y;

	mouse_y = (mouse_y < 0) ? 0 : mouse_y;
	mouse_y = (mouse_y >= HEIGHT) ? HEIGHT : mouse_y;
	mouse_x = (mouse_x < 0) ? 0 : mouse_x;
	mouse_x = (mouse_x >= WIDHT) ? WIDHT : mouse_x;
      break;
  }
}

void mouse_wait(byte a_type) //unsigned char
{
  int _time_out=100000; //unsigned int
  if(a_type==0)
  {
    while(_time_out--) //Data
    {
      if((inportb(0x64) & 1)==1)
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
      if((inportb(0x64) & 2)==0)
      {
        return;
      }
    }
    return;
  }
}

void mouse_write(byte a_write) //unsigned char
{
  //Wait to be able to send a command
  mouse_wait(1);
  //Tell the mouse we are sending a command
  outportb(0x64, 0xD4);
  //Wait for the final part
  mouse_wait(1);
  //Finally write
  outportb(0x60, a_write);
}

byte mouse_read()
{
  //Get's response from mouse
  mouse_wait(0);
  return inportb(0x60);
}

void mouse_init()
{
  byte _status;  //unsigned char

  //Enable the auxiliary mouse device
  mouse_wait(1);
  outportb(0x64, 0xA8);
 
  //Enable the interrupts
  mouse_wait(1);
  outportb(0x64, 0x20);
  mouse_wait(0);
  _status=(inportb(0x60) | 2);
  mouse_wait(1);
  outportb(0x64, 0x60);
  mouse_wait(1);
  outportb(0x60, _status);
 
  //Tell the mouse to use default settings
  mouse_write(0xF6);
  mouse_read();  //Acknowledge
 
  //Enable the mouse
  mouse_write(0xF4);
  mouse_read();  //Acknowledge

  //Setup the mouse handler
  // irq_install_handler(12, mouse_handler);
	set_evec(12 + IRQBASE, (uint32)mouse_handler_irq);
}



extern void print_text_on_buffer(unsigned char x, unsigned char y, char *text);
extern void borrar(int x1, int y1, int x2, int y2);

process mouse (
                did32   dev             /* ID of tty device from which  */
        )                               /*   to accept commands         */
{
	int old_x=0;
	int old_y=0;;
	unsigned char c;
	printf("ahora\n");
	//mouse_install();
	printf("ahora2\n");
	char * h = "hola mundo";
	print_text_on_buffer(10, 10, h);
	char texto[10];
	
	while(1) {
		//borrar(0,0,300,300);
		 sprintf(texto, "x: %d  y: %d \n", mouse_x, mouse_y);
		pixel(old_x, old_y, 0x00ffff00);
		pixel(mouse_x, mouse_y, 0x00ff0000);
		print_text_on_buffer(10, 10, texto);
		old_x = mouse_x; old_y = mouse_y;
		// printf("mouse: 0x%x\n", c);
		c = 0;
		// mouse_write(c);
		sleepms(200);

	}

}

