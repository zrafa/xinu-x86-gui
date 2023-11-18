
/* source: https://forum.osdev.org/viewtopic.php?t=10247 */

#include <xinu.h>
#include <gui.h>

#define GREY 0x00808080
#define WHITE 0x00f0f0f0
#define YELLOW 0x00ffff00
#define BLACK 0x00303030
#define RED 0x00ff0000

extern uint32 nina[];

process gui_test(void)
{
	open(VGA, NULL, 0);	

	gui_draw_hollow_rect(290, 90, 400, 380, GREY);
	gui_draw_rect(300, 100, 380, 360, GREY);
	gui_draw_image(380, 200, 200, 185, nina);
	gui_print_text(320, 120, "Window", WHITE, GREY);

	close(VGA);
	return 0;
}

