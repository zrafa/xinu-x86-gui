
#ifndef GUI_H
#define GUI_H

uint32 rgb16_to_rgb32(uint16 color);
void gui_set_pixel(unsigned x, unsigned y, uint16 color);
void gui_pixel(unsigned x, unsigned y, uint32 color);
void gui_paint_screen();
void gui_draw_char(unsigned int x, unsigned int y, char c, uint32 color,
                                   uint32 bg_color);
void gui_print_text(unsigned int x, unsigned int y, char *text, uint32 color, uint32 bg_color);
void gui_draw_image(int x, int y, int h, int w, const uint32 *image);
void gui_draw_rect(int x, int y, int w, int h, uint32 color);
void gui_draw_hollow_rect(int x, int y, int w, int h, uint32 color);

#endif
