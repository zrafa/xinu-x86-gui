/*  main.c  - main */

#include <xinu.h>
#include <gui.h>

process main(void)
{
	gui_init();

	recvclr();
	resume(create(shell, 8192, 50, "shell", 1, CONSOLE));
	resume(create(microui, 16384, 20, "microui", 0));
	resume(create(vt, 4096, 20, "vt 0", 0));
	resume(create(mu_clock, 4096, 20, "clock", 0));

	/* Wait for shell to exit and recreate it */

	while (TRUE)
	{
		receive();
		sleep(5);
		sleepms(200);
		kprintf("\n\nMain process recreating shell\n\n");
		resume(create(shell, 4096, 20, "shell", 1, CONSOLE));
	}
	return OK;
}
