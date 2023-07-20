#include <xinu.h>
#include <microui.h>

#define NINA_W 200
#define NINA_H 185
#define FRAME_SPACE_W 10
#define FRAME_SPACE_H 35

extern uint32 nina[];
uint8 * buf_nina;

void copy_to_buf(void * dest, void * orig, int w_orig, int w, int h)
{
	int i;

	for (i=0; i<h; i++) {
		memcpy(dest, orig, w*4);	/* 4 because 32bpp */
	        dest = dest + w*4;
		orig = orig + w_orig*4;
	}
}


/* window of this program, called by microui.
 * It draws an image surface (buf_nina).
 * buf_nina might be modified, probably, in main program (mu_gui_demo()) 
 */
void nina_window(mu_Context *ctx) {
	mu_Rect rect;
	int w,h;

	/* do window */
	if (mu_begin_window(ctx, "Demo Nina", mu_rect(700, 40, NINA_W+FRAME_SPACE_W, NINA_H+FRAME_SPACE_H))) {
		mu_Container *win = mu_get_current_container(ctx);
		w = win->rect.w - FRAME_SPACE_W;
		h = win->rect.h - FRAME_SPACE_H;

		if (w > NINA_W)
			w = NINA_W;
		if (h > NINA_H)
			h = NINA_H;

		copy_to_buf(buf_nina, nina, NINA_W, w, h);
		rect = mu_layout_next(ctx);
		mu_draw_image(ctx, buf_nina, mu_rect(rect.x, rect.y, w, h));

		mu_end_window(ctx);
	}
}

process mu_gui_demo(void)
{
	int n;

	buf_nina = getmem(NINA_W * NINA_H * 4);  /* 4 is size for uint32 */
	n = mu_add_win(nina_window);

	/* program source code (for example, modify surface
	 * drawn into window
	 */

	/* wait until window closes or program finishes */
	sleep(100);	

	mu_free_win(n) ;
	freemem(buf_nina, NINA_W * NINA_H * 4);  /* 4 is size for uint32 */
}

