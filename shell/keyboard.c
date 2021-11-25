#include<xinu.h>
#include<stdio.h>

#define KEYBOARD_DATAREG 0x60  // :Data Register(Read\Write)
#define KEYBOARD_CMDREG 0x64   // :Command Register(Read\Write)

#define inportb(p)      inb(p)
#define outportb(p,v)   outb(p,v)


unsigned int keyboard_restart()
{    
   int data = inportb(0x61);     
   outportb(0x61,data | 0x80);//Disables the keyboard  
   outportb(0x61,data & 0x7F);//Enables the keyboard  
   return 0;
}

unsigned char get_scancode()
{
    unsigned char inputdata;
    inputdata = inportb(KEYBOARD_DATAREG);
    return inputdata;
}

unsigned char kblayout [128];  // { ... } Fill your layout yourself 
void keyboard_handler(struct regs *r)
{ 
	char t[80];
     unsigned char scancode; 
     unsigned int shift_key = 0;
	int i = 10;
     scancode = get_scancode();
		sprintf(t, "tecla: 0x%x    \n", scancode);
		// borrar (0,300,600,800);
		print_text_on_buffer(10, 300, t);
     if(scancode == 0x2A)     
     {
          shift_key = 1;//Shift key is pressed
     }      
     else if(scancode & 0xAA)
     {
          int shift_key= 0;//Shift Key is not pressed
     }      
     else    
     {          
         if (scancode & 0x80)
         {
              int shiftaltctrl = 1;//Put anything to see what special keys were pressed
         }
         else
         {  
		//t[0] = kblayout[scancode];
		//t[1] = 0;
//		sprintf(t, "tecla: %d \n", kblayout[scancode]);
		// borrar (0,300,600,800);
//		print_text_on_buffer(10, 300, t);
              //printtxt(kblayout[scancode]); //Prints the character which was pressed         
         }     
     }
}



void keyboard_wait(byte a_type) //unsigned char
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

void keyboard_write(byte a_write) //unsigned char
{
  //Wait to be able to send a command
  keyboard_wait(1);
  //Tell the keyboard we are sending a command
  outportb(0x64, 0xD4);
  //Wait for the final part
  keyboard_wait(1);
  //Finally write
  outportb(0x60, a_write);
}

byte keyboard_read()
{
  //Get's response from mouse
  keyboard_wait(0);
  return inportb(0x60);
}



void keyboard_init()
{
	int i;
	for (i=0; i<128; i++)
		kblayout[i] = i;
	//keyboard_restart();


  byte _status;  //unsigned char

  //Enable the keyboard device   old: auxiliary mouse device
  keyboard_wait(1);
  outportb(0x64, 0xAE);
 
  //Enable the interrupts
  keyboard_wait(1);
  outportb(0x64, 0x20);
  keyboard_wait(0);
  _status=(inportb(0x60) | 1);
  keyboard_wait(1);
  outportb(0x64, 0x60);
  keyboard_wait(1);
  outportb(0x60, _status);
 
  //Tell the keyboard to use default settings
/* while((inportb(0x64)&2)!=0){};*/
 // outportb(0x64, 0xF6);
  //keyboard_read();  //Acknowledge
 
  //Enable the keyboard
/*while((inportb(0x64)&2)!=0){}; */


	set_evec(1 + IRQBASE, (uint32)keyboard_handler_irq);
  //outportb(0x64, 0xF4);
  //keyboard_read();  //Acknowledge
};


process keyboard (
                did32   dev             /* ID of tty device from which  */
        )                               /*   to accept commands         */
{
	//keyboard_init();

	while(1) {

		sleepms(400);
	}
}
