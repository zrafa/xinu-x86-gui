/*  main.c  - main */

#include <xinu.h>


extern paint_screen();
extern print_text_on_vga(unsigned int x, unsigned int y, const char * t);

process	main(void)
{


	paint_screen();
	print_text_on_vga(10, 200, "Xinu OS for PC with VGA support");
	print_text_on_vga(10, 220, "Sistemas Operativos I");

	recvclr();
	resume(create(shell, 8192, 50, "shell", 1, CONSOLE));
	resume(create(mouse_p, 1024, 50, "mouse", 1, CONSOLE));
	resume(create(keyboard_p, 1024, 50, "keyboard", 1, CONSOLE));

	/* Wait for shell to exit and recreate it */

	while (TRUE) {
		receive();
		sleepms(200);
		kprintf("\n\nMain process recreating shell\n\n");
		resume(create(shell, 4096, 20, "shell", 1, CONSOLE));
	}
	return OK;
    
}
