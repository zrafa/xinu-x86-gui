#include <xinu.h>
#include <visual.h>
#include <microui.h>
#include <gui.h>

#define WIDTH 240
#define HEIGHT 160
#define FRAME_SPACE_W 10
#define FRAME_SPACE_H 35
#define RED 0x00ff0000

extern process visual_dev();

uint32 * visual_buf;
uint32 * spawn_buf;
uint32 width;
uint32 height;

int submitted_w = 0;
int submitted_h = 0;
int submitted_window = 0;
int submitted_rrgb = 0;
int submitted_grgb = 0;
int submitted_brgb = 0;

int submitted_graphics = -1;
int must_clear = 0;

int actual_width = 0;
int actual_height = 0;
int actual_rrgb = 0;
int actual_grgb = 0;
int actual_brgb = 0;
static char size_w[5] = "\0";
static char size_h[5] = "\0";
static char size_rrgb[4] = "\0";
static char size_grgb[4] = "\0";
static char size_brgb[4] = "\0";

extern uint32 nina[];
extern uint16 space[];
extern uint16 red[];

#define NINA_W 200
#define NINA_H 185
#define FRAME_SPACE_W 10
#define FRAME_SPACE_H 35

    

int katoi(char * str) {
  int res = 0;
  for (int i = 0; str[i] != '\0'; i++) {
    if (str[i] > '9' || str[i] < '0') {
      continue;
    }
    res = res * 10 + (str[i] - '0');
  }

  return res;
}

void spawned_window(struct SpawnedWindow sp, mu_Context * ctx) {
     //mu_Rect rect;

     if (sp.width == 0) {
      return;
     }
    mu_Rect rect = mu_rect(700, 200, sp.width, sp.height);
    mu_Color test_color = mu_color(1, 250, 1, 1);

    //mu_Rect colored_rect =
    //rect = mu_draw_rect(ctx, rect, sp.color);
    if (mu_begin_window(ctx, "Customizable!", rect)) {
      //mu_Rect r = mu_layout_next(ctx);
      //mu_draw_rect(ctx, r, mu_color(bg[0], bg[1], bg[2], 255));
      
      //gui_paint_screen(mu_color_to_rgb32(test_color));
     
      mu_end_window(ctx);
    //}
    }
}

process graphics_window_process(struct GraphicsWindow gw) {
  /* int n;
	uint32 *buf;

	buf = gui_buf_getmem(NINA_W*NINA_H*4);
	gui_buf_draw_image(buf, NINA_W, 0, 0, NINA_W, NINA_H, nina);
//	buf = getmem(NINA_W*NINA_H*4);
//	memcpy(buf, nina, NINA_W*NINA_H*4);
	n = mu_add_win("Demo Nina", 700, 40, NINA_W, NINA_H, buf);

	mu_event_t e;
	for (;;) {
		mu_get_event(n, &e);
                if (e.but != -1)
                        printf("mouse x: %d, y: %d \n", e.mouse.x, e.mouse.y);
                if (e.c[0] != '\0')
                        printf("KEY: %c %d \n", e.c[0], e.c[0]);

	};
	sleep(100);	

	gui_buf_freemem(buf, NINA_W*NINA_H*4);
	mu_free_win(n) ; */
}

void graphics_window(struct GraphicsWindow gw, mu_Context * ctx) {
  //resume(create(graphics_window_process, 13224, 20, "graphics", 1, gw));
}


struct SpawnedWindow mu_visual(mu_Context * ctx) {

  if (mu_begin_window(ctx, "Visual Developer", mu_rect(450, 300, 300, 200))) {
    mu_layout_row(ctx, 2, (int[]) { -70, -1 }, 0);
    //mu_text(ctx, size_w);
    if (mu_textbox(ctx, size_w, sizeof(size_w))) {
      mu_set_focus(ctx, ctx->last_id);
      //submitted_w = 1;
      //if (mu_button(ctx, "x")) { printf("hola"); write_log("Pressed button 1"); }
    }
    if (mu_button(ctx, "Enter Width")) { submitted_w = 1; }


    // height customization
    mu_layout_row(ctx, 2, (int[]) { -70, -1 }, 0);
    //mu_text(ctx, size_w);
    if (mu_textbox(ctx, size_h, sizeof(size_h))) {
      mu_set_focus(ctx, ctx->last_id);
      //submitted_h = 1;
      //if (mu_button(ctx, "x")) { printf("hola"); write_log("Pressed button 1"); }
    }
    if (mu_button(ctx, "Enter Height")) { submitted_h = 1; }
  
  mu_layout_row(ctx, 2, (int[]) { -70, -1 }, 0);
   if (mu_textbox(ctx, size_rrgb, sizeof(size_rrgb))) {
      mu_set_focus(ctx, ctx->last_id);
      //submitted_h = 1;
      //if (mu_button(ctx, "x")) { printf("hola"); write_log("Pressed button 1"); }
    }
    if (mu_button(ctx, "Enter Redness")) {submitted_rrgb = 1;}

mu_layout_row(ctx, 2, (int[]) { -70, -1 }, 0);
    if (mu_textbox(ctx, size_grgb, sizeof(size_grgb))) {
      mu_set_focus(ctx, ctx->last_id);
      //submitted_h = 1;
      //if (mu_button(ctx, "x")) { printf("hola"); write_log("Pressed button 1"); }
    }
    if (mu_button(ctx, "Enter Greenness")) {submitted_grgb = 1;}

mu_layout_row(ctx, 2, (int[]) { -70, -1 }, 0);
    if (mu_textbox(ctx, size_brgb, sizeof(size_brgb))) {
      mu_set_focus(ctx, ctx->last_id);
      //submitted_h = 1;
      //if (mu_button(ctx, "x")) { printf("hola"); write_log("Pressed button 1"); }
    }
    if (mu_button(ctx, "Enter Blueness")) {submitted_brgb = 1;}
    mu_layout_row(ctx, 2, (int[]) { -70, -1 }, 0);
    if (mu_button(ctx, "Submit")) { submitted_window = 1; }
    if (mu_button(ctx, "Change background to Nina for fun")) {
        submitted_graphics = 1;
    }

    if (submitted_window) {
      // spawn new window
      //int spawn = create(spawned_window, 16384, 20, "Spawned Window", 2, actual_width, actual_height);
    //resume(spawn);
      
      actual_height = katoi(size_h);
      actual_width = katoi(size_w);
      actual_rrgb = katoi(size_rrgb);
      actual_grgb = katoi(size_grgb);
      actual_brgb = katoi(size_brgb);
      struct SpawnedWindow sp;
      sp.width = actual_width;
      sp.height = actual_height;
      sp.color = mu_color(actual_rrgb, actual_grgb, actual_brgb, 255);
      sp.graphic = 0;
      mu_end_window(ctx);
      must_clear = 1;
      return sp;
    }

  /* for (int i = 0; i < 1000; i++) {
    mu_push_id(ctx, &i, sizeof(i));
    if (mu_button(ctx, "x")) {
      printf("Pressed button %d\n", i);
    }
    mu_pop_id(ctx);
  } */
      //mu_end_window(ctx);
      if (must_clear == 1) {
        size_h[0] = '\0';
        size_w[0] = '\0';
        size_brgb[0] = '\0';
        size_rrgb[0] = '\0';
        size_grgb[0] = '\0';
        must_clear = 0;
        submitted_window = 0;
      }
      struct SpawnedWindow sp;
      sp.width = 0;
      sp.height = 0;
      sp.color = mu_color(actual_rrgb, actual_grgb, actual_brgb, 0);
      if (submitted_graphics == 1) {
        sp.graphic = 1;
      } else {
        sp.graphic = 0;
      }
      mu_end_window(ctx);
      return sp;
  }

    
}