#include <xinu.h>
#include <gui.h>

process	mousepr(int * mouse_buf_pt)
{
	open(MOUSE, NULL, 0);

	while (TRUE) {
        read(MOUSE, mouse_buf_pt, sizeof(mousec.mouse));
        gui_signal_event_type(GUI_EVENT_MOUSE);  // Wake GUI for mouse input
    }

	close(MOUSE);

    return OK;
}
