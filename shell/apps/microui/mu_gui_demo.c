#include <xinu.h>
#include <microui.h>

#define NINA_W 200
#define NINA_H 185
#define FRAME_SPACE_W 10
#define FRAME_SPACE_H 35

extern uint32 nina[];

/* window of this program, called by microui.
 * It draws an image surface (buf_nina).
 * buf_nina might be modified, probably, in main program (mu_gui_demo()) 
 */
void nina_window(mu_Context *ctx) {
	mu_Rect rect;
	int w,h;
	mu_Vec2 mouse;
	char c;

	/* do window */
	if (mu_begin_window(ctx, "Demo Nina", mu_rect(700, 40, NINA_W+FRAME_SPACE_W, NINA_H+FRAME_SPACE_H))) {
		mu_Container *win = mu_get_current_container(ctx);
		w = win->rect.w - FRAME_SPACE_W;
		h = win->rect.h - FRAME_SPACE_H;

		if (w > NINA_W)
			w = NINA_W;
		if (h > NINA_H)
			h = NINA_H;

		rect = mu_layout_next(ctx);
		mu_draw_image(ctx, nina, mu_rect(rect.x, rect.y, NINA_W, NINA_H), w, h, &mouse, &c);

		mu_end_window(ctx);
	}
}

process mu_gui_demo(void)
{
	int n;
	uint32 *buf;

	buf = getmem(NINA_W*NINA_H*4);
	memcpy(buf, nina, NINA_W*NINA_H*4);
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

	freemem(buf, NINA_W*NINA_H*4);
	mu_free_win(n) ;
}

