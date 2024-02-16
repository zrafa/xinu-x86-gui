#include <xinu.h>

process	kbdpr(unsigned char *curr_key_pt)
{
	open(KEYBOARD, NULL, 0);

	while (TRUE) {
		read(KEYBOARD, curr_key_pt, 1);     
    }

	close(KEYBOARD);

    return OK;
}
