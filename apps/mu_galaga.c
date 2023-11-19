#include <xinu.h>
#include <microui.h>

#define GALAGA_W 240
#define GALAGA_H 160
#define FRAME_SPACE_W 10
#define FRAME_SPACE_H 35

extern process galaga();

//extern uint32 *galaga_buf;
uint32 *galaga_buf;

/* window of this program, called by microui.
 * It draws an image surface (galaga_buf).
 * galaga_buf might be modified, probably, in main program (mu_gui_demo()) 
 *
 * int n argument is the window number n.
 */
void galaga_window(mu_Context *ctx, int n) {
	mu_Rect rect;
	int w,h;

	/* do window */
	if (mu_begin_window(ctx, "Galaga", mu_rect(700, 200, GALAGA_W+FRAME_SPACE_W, GALAGA_H+FRAME_SPACE_H))) {
		mu_Container *win = mu_get_current_container(ctx);
		w = win->rect.w - FRAME_SPACE_W;
		h = win->rect.h - FRAME_SPACE_H;

		if (w > GALAGA_W)
			w = GALAGA_W;
		if (h > GALAGA_H)
			h = GALAGA_H;
                mu_draw_image(ctx, galaga_buf, mu_rect(rect.x, rect.y, GALAGA_W, GALAGA_H), w, h, n);

		mu_end_window(ctx);
	}
}



process mu_galaga(void)
{
	int n;

//	resume(create(galaga, 8124, 20, "galaga", 0));
//	n = mu_add_win(galaga_window);
        galaga_buf = getmem(GALAGA_W*GALAGA_H*4);
	n = mu_add_win("Galaga", 700, 200, GALAGA_W, GALAGA_H, galaga_buf);
	galaga(n);

	/* program source code (for example, modify surface
	 * drawn into window
	 */

	/* wait until window closes or program finishes */
//	cra
// sleep(100);	
        freemem(galaga_buf, GALAGA_W*GALAGA_H*4);
	mu_free_win(n) ;
}

