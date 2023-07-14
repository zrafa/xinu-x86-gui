#include <xinu.h>
#include "microui.h"

extern uint32 nina[];

/* window of this program, called by microui */
void nina_window(mu_Context *ctx) {
	mu_Rect rect;
	mu_Rect nina_r = mu_rect(0, 0, 185, 200);

	/* do window */
	if (mu_begin_window(ctx, "Demo Nina", mu_rect(700, 40, nina_r.w+30, nina_r.h+20))) {
		rect = mu_layout_next(ctx);
		mu_draw_image(ctx, nina, mu_rect(rect.x, rect.y, nina_r.w, nina_r.h));

		mu_end_window(ctx);
	}
}


process mu_gui_demo(void)
{
	int n;

	n = mu_add_win(nina_window);
	/* program source code (for example, modify surface
	 * drawn into window
	 */
	sleep(10);

	/* wait until window closes or program finishes */
	mu_free_win(n) ;
}

