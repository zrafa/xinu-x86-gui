#include <xinu.h>
#include <gui.h>

process	kbdpr(unsigned char *curr_key_pt)
{
	open(KEYBOARD, NULL, 0);

	while (TRUE) {
		read(KEYBOARD, curr_key_pt, 1);
		gui_signal_event_type(GUI_EVENT_KEYBOARD);  // Wake GUI for keyboard input
    }

	close(KEYBOARD);

    return OK;
}
