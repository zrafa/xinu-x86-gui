
#ifndef GUI_H
#define GUI_H

#define GUI_FLUSH	seek(VGA, 0); write(VGA, gui_buf, gui_width * gui_heigth * gui_bpp);

/* Event types for GUI updates */
#define GUI_EVENT_KEYBOARD    0x01  // Key pressed
#define GUI_EVENT_MOUSE       0x02  // Mouse moved/clicked
#define GUI_EVENT_VT_OUTPUT   0x04  // Terminal has output
#define GUI_EVENT_APP_UPDATE  0x08  // App updated its buffer

extern uint32 *gui_buf;
extern int32 gui_buf_size;
extern int32 gui_bpp;
extern int32 gui_width;
extern int32 gui_height;

/* Event system */
extern sid32 gui_event_sem;
void gui_events_init(void);
void gui_signal_event(void);
void gui_signal_event_type(uint32 event_type);
uint32 gui_get_pending_events(void);
void gui_signal_redraw(int win_id);


void gui_set_pixel(int x, int y, uint16 color);
void gui_pixel(int x, int y, uint32 color);
void gui_paint_screen(uint32 color);
void gui_draw_char(int x, int y, char c, uint32 color,
                                   uint32 bg_color);
void gui_print_text(int x, int y, char *text, uint32 color, uint32 bg_color);
void gui_draw_image(int x, int y, int w, int h, uint32 *image);
void gui_draw_rect(int x, int y, int w, int h, uint32 color);
void gui_draw_hollow_rect(int x, int y, int w, int h, uint32 color);
void gui_init(void);
void gui_flush(void);
char check_bit(unsigned char c, int pos);

#endif
