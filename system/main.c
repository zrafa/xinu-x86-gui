/*  main.c  - main */

#include <xinu.h>

#define YELLOW 0x00ffff00
#define BLACK 0x00303030

process main(void)
{
	gui_paint_screen();
	gui_print_text(10, 200, "Xinu OS for x86 PC with VGA, mouse and keyboard support.", BLACK, YELLOW);

	recvclr();
	resume(create(shell, 8192, 50, "shell", 1, CONSOLE));
	resume(create(mouse_p, 1024, 50, "mouse", 1, CONSOLE));
	resume(create(keyboard_p, 1024, 50, "keyboard", 1, CONSOLE));

	/* Wait for shell to exit and recreate it */

	while (TRUE)
	{
		receive();
		sleepms(200);
		kprintf("\n\nMain process recreating shell\n\n");
		resume(create(shell, 4096, 20, "shell", 1, CONSOLE));
	}
	return OK;
}
