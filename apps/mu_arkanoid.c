#include <xinu.h>
#include <microui.h>

#define ARKANOID_W 240
#define ARKANOID_H 240
#define FRAME_SPACE_W 10
#define FRAME_SPACE_H 35

//extern int arkanoid(uint32* buf, int w, int n);
//int arkanoid(uint32* buf, int w, int n){
//}


//extern uint32 *arkanoid_buf;
uint32 *arkanoid_buf;

/* window of this program, called by microui.
 * It draws an image surface (arkanoid_buf).
 * arkanoid_buf might be modified, probably, in main program (mu_gui_demo()) 
 *
 * int n argument is the window number n.
 */
void arkanoid_window(mu_Context *ctx, int n) {
	mu_Rect rect;
	int w,h;

	/* do window */
	if (mu_begin_window(ctx, "Arkanoid", mu_rect(700, 200, ARKANOID_W+FRAME_SPACE_W, ARKANOID_H+FRAME_SPACE_H-80))) {
		mu_Container *win = mu_get_current_container(ctx);
		w = win->rect.w - FRAME_SPACE_W;
		h = win->rect.h - FRAME_SPACE_H-80;

		if (w > ARKANOID_W)
			w = ARKANOID_W;
		if (h > ARKANOID_H-80)
			h = ARKANOID_H-80;
                mu_draw_image(ctx, arkanoid_buf, mu_rect(rect.x, rect.y, ARKANOID_W, ARKANOID_H-80), w, h, n);

		mu_end_window(ctx);
	}
}



process mu_arkanoid(void)
{
	int n;

//	resume(create(arkanoid, 8124, 20, "arkanoid", 0));
//	n = mu_add_win(arkanoid_window);
        arkanoid_buf = getmem(ARKANOID_W*ARKANOID_H*4);
	n = mu_add_win("Arkanoid", 700, 200, ARKANOID_W, ARKANOID_H-80, arkanoid_buf);
	arkanoid(arkanoid_buf, ARKANOID_W, n);

	/* program source code (for example, modify surface
	 * drawn into window
	 */

	/* wait until window closes or program finishes */
//	cra
// sleep(100);	
        freemem(arkanoid_buf, ARKANOID_W*ARKANOID_H*4);
	mu_free_win(n) ;
}

