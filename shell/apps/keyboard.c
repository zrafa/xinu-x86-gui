#include <xinu.h>
#include <stdio.h>
#include <string.h>
#include <gui.h>

#define YELLOW 0x00ffff00
#define BLACK 0x00303030

process keyboard_p (
                did32   dev             /* ID of tty device from which  */
        )                               /*   to accept commands         */
{
	unsigned char c;

	/*
	char * h = "Xinu x86 GUI experiments: VGA display, mouse and kbd";
	gui_print_text_on_vga(10, 500, h);
	*/
	
	char texto[10];
	char buf;
	
	open(KEYBOARD, NULL, 0);
	while(1) {
		read(KEYBOARD, &buf, 1);

		unsigned char record_key = buf;

		// print message on screen
		char message_on_screen[80];
		sprintf(message_on_screen, "kbd: 0x%x     ", record_key);
		gui_print_text(10, 300, message_on_screen, BLACK, YELLOW);


		unsigned int shift_key = 0;
		if(record_key == 0x2A) {
			shift_key = 1;//Shift key is pressed
		} else if(record_key & 0xAA) {
			shift_key = 0;//Shift Key is not pressed
		} else {          
			if (record_key & 0x80) {
				int shiftaltctrl = 1;//Put anything to see what special keys were pressed
			} else {
			}     
		}

		sleepms(10);

	}
	close(KEYBOARD);

}

