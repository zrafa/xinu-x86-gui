#ifndef RENDERER_H
#define RENDERER_H

#include "microui.h"

void r_init(void);
void r_draw_rect(mu_Rect rect, mu_Color color);
void r_draw_text(const char *text, mu_Vec2 pos, mu_Color color);
void r_draw_icon(int id, mu_Rect rect, mu_Color color);
 int r_get_text_width(const char *text, int len);
 int r_get_text_height(void);
void r_set_clip_rect(mu_Rect rect);
void r_clear(mu_Color color);
void r_present(void);
extern const unsigned short pointer[];
struct mouse_state {
	int pos_y;
    int pos_x;
    uint8 left_mouse;
    uint8 right_mouse;
};
extern struct mouse_state m_state;

void r_handle_input(mu_Context *ctx);
#endif

