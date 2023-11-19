#include <xinu.h>
#include <microui.h>
#include <gui_buf.h>

#define NINA_W 200
#define NINA_H 185
#define FRAME_SPACE_W 10
#define FRAME_SPACE_H 35

extern uint32 nina[];


process mu_gui_demo(void)
{
	int n;
	uint32 *buf;

	buf = gui_buf_getmem(NINA_W*NINA_H*4);
	gui_buf_draw_image(buf, NINA_W, 0, 0, NINA_W, NINA_H, nina);
//	buf = getmem(NINA_W*NINA_H*4);
//	memcpy(buf, nina, NINA_W*NINA_H*4);
	n = mu_add_win("Demo Nina", 700, 40, NINA_W, NINA_H, buf);

	/* program source code (for example, modify surface
	 * drawn into window
	 */

	mu_event_t e;
	for (;;) {
		mu_get_event(n, &e);
                if (e.but != -1)
                        printf("mouse x: %d, y: %d \n", e.mouse.x, e.mouse.y);
                if (e.c[0] != '\0')
                        printf("KEY: %c %d \n", e.c[0], e.c[0]);

	};

	/* wait until window closes or program finishes */
	sleep(100);	

	gui_buf_freemem(buf, NINA_W*NINA_H*4);
	mu_free_win(n) ;
}

