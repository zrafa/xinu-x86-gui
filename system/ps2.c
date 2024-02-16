/* ps2.c  -  ps2_wait, ps2_read, ps2_write */

#include <xinu.h>

void ps2_wait(unsigned char a_type)
{
    unsigned int _time_out = 100000; // unsigned int

    if(a_type == 0)
    {
        while(_time_out--) //Data
        {
            if((inb(PS2_CTRL_STATUS_PORT) & 1)==1)
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
            if((inb(PS2_CTRL_STATUS_PORT) & 2)==0)
            {
                return;
            }
        }
        return;
    }
}

void ps2_write(unsigned char a_write)
{
    // Wait to be able to send a command
    ps2_wait(1);
    // Tell the controller we are sending a command
    outb(PS2_CTRL_STATUS_PORT, WRITE_AUX_DEVICE);

    // Wait for the final part
    ps2_wait(1);
    // Finally write
    outb(PS2_DATA_PORT, a_write);
}

unsigned char ps2_read()
{
    // Get's response from controller
    ps2_wait(0);
    return inb(PS2_DATA_PORT);
}


