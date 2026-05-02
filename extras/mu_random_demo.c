#include <xinu.h>
#include <microui.h>
#include <gui_buf.h>
#include <gui.h>

/* size of the window */
#define RANDOM_W 200
#define RANDOM_H 200

#define FRAME_SPACE_W 10
#define FRAME_SPACE_H 35

//extern uint32 nina[];


process mu_gui_random(void)
{
	int n;
	uint32 *buf;

	/* window mem buf (where you put/draw pixels) */
	buf = gui_buf_getmem(RANDOM_W*RANDOM_H*4);

	/* create the window */
	n = mu_add_win("Demo pixeles random", 700, 40, RANDOM_W, RANDOM_H, buf);

	/* copy/put an image inside the window mem buf: */
	//gui_buf_draw_image(buf, RANDOM_W, 0, 0, RANDOM_W, RANDOM_H, nina);
	
	/* program source code (for example, modify surface
	 * drawn into window, etc)
	 */

	mu_event_t e;
	long long color = 0;

	for (;;) {
		color++;
		if (color == 65000) color = 0;

		for (int i=0; i<RANDOM_W; i++)
		for (int j=0; j<RANDOM_H; j++) {

			gui_buf_pixel(buf, RANDOM_W, i, j, color);
		}

		gui_signal_redraw(n);

		mu_get_event(n, &e);
                if (e.but != -1)
                        printf("mouse x: %d, y: %d \n", e.mouse.x, e.mouse.y);
                if (e.c[0] != '\0')
                        printf("KEY: %c %d \n", e.c[0], e.c[0]);

		sleepms(2);
	};

	/* wait until window closes or program finishes */
	sleep(100);	

	gui_buf_freemem(buf, RANDOM_W*RANDOM_H*4);
	mu_free_win(n) ;
}

