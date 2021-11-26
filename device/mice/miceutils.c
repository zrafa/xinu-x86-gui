/* source: https://forum.osdev.org/viewtopic.php?t=10247 */

#include <xinu.h>
#include <mouse.h>

//Mouse.inc by SANiK
//License: Use as you wish, except to cause damage

void mouse_wait(unsigned char a_type) //unsigned char
{
  int _time_out=100000; //unsigned int
  if(a_type==0)
  {
    while(_time_out--) //Data
    {
      if((inportb(MOUSE_CTRL_STATUS_PORT) & 1)==1)
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
      if((inportb(MOUSE_CTRL_STATUS_PORT) & 2)==0)
      {
        return;
      }
    }
    return;
  }
}

void mouse_write(unsigned char a_write) //unsigned char
{
  //Wait to be able to send a command
  mouse_wait(1);
  //Tell the mouse we are sending a command
  outportb(MOUSE_CTRL_STATUS_PORT, 0xD4);
  //Wait for the final part
  mouse_wait(1);
  //Finally write
  outportb(MOUSE_DATA_PORT, a_write);
}

unsigned char mouse_read()
{
  //Get's response from mouse
  mouse_wait(0);
  return inportb(MOUSE_DATA_PORT);
}

