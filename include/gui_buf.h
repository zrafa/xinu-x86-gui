
#ifndef GUI_BUF_H
#define GUI_BUF_H

uint32 rgb16_to_rgb32(uint16 color);

uint32 * gui_buf_getmem(int size);

void gui_buf_freemem(uint32 *buf, int size);

void gui_buf_pixel(uint32 *buf, int w, int x, int y, uint32 color);

void gui_buf_rect(uint32 *buf, int width_buf, int x, int y, int width, int heigth, uint32 color);

void gui_buf_hollow_rect(uint32 *buf, int width_buf, int x, int y, int width, int height, uint32 color);

void gui_buf_draw_image(uint32 *buf, int width_buf, int x, int y, int width, int height, uint32 *image);

void gui_buf_draw_char(uint32 *buf, int width_buf, int x, int y, char c, uint32 color,
                                   uint32 bg_color);

void gui_buf_print_text(uint32 *buf, int width_buf, int x, int y, char *text, uint32 color, uint32 bg_color);



#endif
