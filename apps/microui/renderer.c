#include <xinu.h>
#include <gui.h>
#include <renderer.h>
#include "atlas2.inl"

#define BUFFER_SIZE 16384

static int width  = 800;
static int height = 600;
static int buf_idx;
struct mouse_state prev_m_state;
struct mouse_state m_state;

uint32 mu_color_to_rgb32(mu_Color color)
{
	/* Construct 32-bit format, we get 0x00RRGGBB by shifting bits */
	uint32 color32 = 0x00ffffff & ((color.r << 16) | (color.g << 8) | color.b);
	return color32;
}


void r_init() {
	/* init mouse states */
	prev_m_state.pos_x = 0;
	prev_m_state.pos_y = 0;
	prev_m_state.left_mouse = 0;
	prev_m_state.left_mouse = 0;
	m_state.pos_x = 0;
	m_state.pos_y = 0;
	m_state.left_mouse = 0;
	m_state.left_mouse = 0;
}


void r_draw_rect(mu_Rect rect, mu_Color color) {
  gui_draw_rect(rect.x, rect.y, rect.w, rect.h, mu_color_to_rgb32(color));
}


void r_draw_text(const char *text, mu_Vec2 pos, mu_Color color) {
  gui_print_text(pos.x, pos.y, text, mu_color_to_rgb32(color), 0);
}


void r_draw_icon(int id, mu_Rect rect, mu_Color color) {
  int c, w, h;
	mu_Vec2 pos;
	char buf[2];
	switch (id) {
	  case MU_ICON_CLOSE:		c = 'x'; break;
	  case MU_ICON_CHECK:		c = 'X'; break;
	  case MU_ICON_COLLAPSED:	c = '>'; break;
	  case MU_ICON_EXPANDED:	c = 'v'; break;
  	}
	buf[0] = c;
	buf[1] = 0;
	w = r_get_text_width(buf, 1);
	h = r_get_text_height();
	pos.x = rect.x + (rect.w - w) / 2;
	pos.y = rect.y + (rect.h - h) / 2;
	r_draw_text(buf, pos, color);
}


void r_draw_image(void * addr, mu_Rect rect, int w, int h) {
	int i;

        for (i=0; i<h; i++) {
		gui_draw_image(rect.x, rect.y+i, w, 1, (uint32 *) addr);
		addr = addr + rect.w*4;		/* 4 because 32bpp */
	}
}


int r_get_text_width(const char *text, int len) {
  return len * 6;
}


int r_get_text_height(void) {
  return 8;
}


void r_set_clip_rect(mu_Rect rect) {
}


void r_clear(mu_Color clr) {
	gui_paint_screen(mu_color_to_rgb32(clr));
}

void r_present(void) {
	gui_flush();
}


extern void write_log(char * );

int mouse_buf[3];
void r_draw_mouse() {
    read(MOUSE, mouse_buf, 3);
	/* check left mouse being pressed */
	m_state.left_mouse = (mouse_buf[0] & 0x01) ? 1 : 0;
	/* update mouse position */
	m_state.pos_x = mouse_buf[1];
	m_state.pos_y = mouse_buf[2];
	/* render */
    gui_draw_image(m_state.pos_x, m_state.pos_y, 11, 18, pointer);
}

unsigned char new_key;
unsigned char prev_key;
unsigned char curr_key;
int shift = 0;
int caps = 0;
int caps_goingto = 0;
int repetition = 0;
#define N_REPETITION 200
#define N_REP_DELAY 20

int is_SHIFT_key(unsigned char c)
{
	int res = 0;
	//if ((c == 13) || (c == 8) || (c == 170) || (c == 179) || (c == 178) || (c == 186) || (c == 180))
	if ((c == 170) || (c == 179) || (c == 178) || (c == 186) || (c == 180))
		res = 1;

	return res;
}
int is_special_key(unsigned char c)
{
	int res = 0;
	if ((c == 13) || (c == 8))
		res = 1;

	return res;
}


void r_handle_input(mu_Context *ctx)
{
	read(KEYBOARD, &curr_key, 1);
	/* get ascii code */
	new_key = (curr_key < 59) ? latin_qwerty_map[curr_key - 1] : 0;
	// printf("TECLA : %d \n", new_key);
	// 186 CAPS LOCK UP
	// Las liberaciones de tecla tienen el bit mas significativo en 1
	// Ejemplo: si tecla q == 0x10, entonces liberar tecla q tiene codigo 0x90

	/* process CAPS */
	if ((new_key == 180) && (caps == 0)) { 		/* CAPS LOCK key going to pressed */
		caps_goingto = 1;
		prev_key = new_key;
	} else if ((curr_key == 186) && (caps_goingto == 1)) {
		caps = 1;
		prev_key = new_key;
	} else if ((new_key == 180) && (caps == 1)) { 		/* CAPS LOCK key going to released */
		caps_goingto = 0;
		prev_key = new_key;
	} else if ((curr_key == 186) && (caps_goingto == 0)) {
		caps = 0;
		prev_key = new_key;

	/* process SHIFT */
	} else if ((new_key == 178) || (new_key == 179)) { 		/* SHIFT keys */
		shift = 1;
		prev_key = new_key;
	} else if (curr_key == 170) {				/* release SHIFT keys */
		shift = 0;
		prev_key = new_key;

	/* SHIFT or CAPS + key */
	} else if ((shift || caps) && (curr_key < 59) &&
		 (!is_SHIFT_key(new_key)) && (!is_special_key(new_key))) {
			new_key = latin_qwerty_map[curr_key - 1 + 76];
		};

	/* make text for mu_input_text */
	char buf[2];
	buf[0] = new_key;
	buf[1] = '\0';


	if (prev_key != new_key) 
		repetition = 0;
	repetition++;
        if (is_SHIFT_key(new_key)) {
		repetition = 0;
		prev_key = new_key;
	}
	
	if ((prev_key != new_key) || ((repetition>N_REPETITION) && ((repetition % N_REP_DELAY) == 0))) {

			// RAFA all the keys must go to virtual terminal
		//if (!is_special_key(new_key))
			mu_input_text(ctx, buf);
		if(new_key > 31 && new_key < 127) {
			/* a key was just pressed */
			// RAFA mu_input_text(ctx, buf);
		} else if (new_key == 13) {
			/* enter */
			/* backspace first (remove return symbol from input text) */
			mu_input_keydown(ctx, MU_KEY_BACKSPACE);
			mu_input_keyup(ctx, MU_KEY_BACKSPACE);
			/* enter */
			mu_input_keydown(ctx, MU_KEY_RETURN);
			mu_input_keyup(ctx, MU_KEY_RETURN);
		} else if (new_key == 8) {
			/* backspace (this first bksp is for removing bksp symbol from input text */
			mu_input_keydown(ctx, MU_KEY_BACKSPACE);
			mu_input_keyup(ctx, MU_KEY_BACKSPACE);
			/* backspace */
			mu_input_keydown(ctx, MU_KEY_BACKSPACE);
			mu_input_keyup(ctx, MU_KEY_BACKSPACE);
		}
		prev_key = new_key;
	}
	/* keep mouse position updated in microui */
	mu_input_mousemove(ctx, m_state.pos_x, m_state.pos_y);
	if(prev_m_state.left_mouse != m_state.left_mouse) {
		/* check if mouse click was recently pressed */
		if(m_state.left_mouse) {
			mu_input_mousedown(ctx, m_state.pos_x, m_state.pos_y, 0x01);
		} else {
			mu_input_mouseup(ctx, m_state.pos_x, m_state.pos_y, 0x01);
		}
		/* update previous mouse state once used*/
		prev_m_state.left_mouse = m_state.left_mouse;
	}
}
