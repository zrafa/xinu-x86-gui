#include <xinu.h>
#include <stdio.h>
#include <string.h>
#include <vga.h>

extern void gui_print_text_on_vga(unsigned int x, unsigned int y, const char *text, uint32 color, uint32 backgroundColor);
extern void borrar(int x1, int y1, int x2, int y2);

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
	unsigned char buf;
	
	open(KEYBOARD, NULL, 0);
	while(1) {
		read(KEYBOARD, &buf, 1);

		unsigned char record_key = buf;

		// print message on screen
		char message_on_screen[80];
		sprintf(message_on_screen, "kbd: 0x%x     ", record_key);
		gui_print_text_on_vga(10, 300, message_on_screen, 0x00303030, 0x00ffff00);


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

