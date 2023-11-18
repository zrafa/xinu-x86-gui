/**
	This file is part of FORTMAX kernel.

	FORTMAX kernel is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	FORTMAX kernel is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with FORTMAX kernel.  If not, see <http://www.gnu.org/licenses/>.

	Copyright: Martin K. Schröder (info@fortmax.se) 2014
*/

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define VT100_CHAR_WIDTH 6
#define VT100_CHAR_HEIGHT 10
#define VT100_SCREEN_WIDTH (VT100_CHAR_WIDTH*80)
#define VT100_SCREEN_HEIGHT (VT100_CHAR_HEIGHT*24)
#define VT100_HEIGHT (VT100_SCREEN_HEIGHT / VT100_CHAR_HEIGHT)
#define VT100_WIDTH (VT100_SCREEN_WIDTH / VT100_CHAR_WIDTH)

#define MAX_COMMAND_ARGS 4

typedef struct vt100 {
        union flags {
                uint8_t val;
                struct {
                        // 0 = cursor remains on last column when it gets there
                        // 1 = lines wrap after last column to next line
                        uint8_t cursor_wrap : 1; 
                        uint8_t scroll_mode : 1;
                        uint8_t origin_mode : 1; 
                }; 
        } flags;
        
        // cursor position on the screen (0, 0) = top left corner. 
        int16_t cursor_x, cursor_y;
        int16_t saved_cursor_x, saved_cursor_y; // used for cursor save restore
        int16_t scroll_start_row, scroll_end_row; 
        // character width and height
        int8_t char_width, char_height;
        // colors used for rendering current characters
        uint16_t back_color, front_color;
        // the starting y-position of the screen scroll
        uint16_t scroll_value; 
        // command arguments that get parsed as they appear in the terminal
        uint8_t narg; uint16_t args[MAX_COMMAND_ARGS];
        // current arg pointer (we use it for parsing) 
        uint8_t carg;
        
        void (*state)(struct vt100 *term, uint8_t ev, uint16_t arg);
        void (*send_response)(struct vt100 *t, char *str);
        void (*ret_state)(struct vt100 *term, uint8_t ev, uint16_t arg); 

        uint32 *buf;
        int width_buf;
	int available;

} vt100_t;


struct vt100 * vt100_get_vt(int n);
int vt100_get_vt_available(uint32 *buf, int width_buf); 
void vt100_free_vt(int n); 
void vt100_init_vts(); 

void vt100_init(struct vt100 *t, void (*send_response)(char *str)); 
void vt100_del_cursor(struct vt100 *t);
void vt100_draw_cursor(struct vt100 *t);
void vt100_putc(struct vt100 *t, uint8 ch);
void vt100_puts(struct vt100 *t, const char *str);

#ifdef __cplusplus
}
#endif
