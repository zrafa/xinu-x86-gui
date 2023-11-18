// /* source: https://forum.osdev.org/viewtopic.php?t=10247 */

#include <xinu.h>
#include <stdio.h>
#include <string.h>
#include <font.h>
#include <gui.h>
#include <gui_buf.h>

/* software double buffering for graphics */
uint32 * gui_buf;

int32 gui_buf_size;
int32 gui_width;
int32 gui_height;
int32 gui_bpp;


void gui_set_pixel(int x, int y, uint16 color)
{
	if ((x >= 240) || (y >= 160))
		return;

	uint32 color32 = rgb16_to_rgb32(color);
	gui_pixel(x, y, color32);
}

/*
 *	The new position is calculated by doing
 *	y * (the total number of pixels in a line) +
 *	x * (the number of bits per pixel divided by 8)
 */
void gui_pixel(int x, int y, uint32 color)
{
	if(x >= 0 && x < gui_width && y >= 0 && y < gui_height)
		gui_buf[y * gui_width + x] = color;
}

void gui_paint_screen(uint32 color)
{
	int x, y;

	for (y = 0; y < gui_height; y++)
	for (x = 0; x < gui_width; x++)
		gui_pixel(x, y, color);
}

char check_bit(unsigned char c, int pos)
{
	unsigned char is_bit_at_pos_on = c & (1 << pos);

	return is_bit_at_pos_on ? 1 : 0;
}

/* Dibuja una letra en el buffer, comenzando con la columna [x,y]
 * para el primer pixel de la letra
 */
void gui_draw_char(int x, int y, char c, uint32 color,
				   uint32 bg_color)
{

	int i;	  /* para recorrer el font por columnas */
	int j;	  /* para recorrer el font por filas */
	char col; /* para almacenar cada columna de pixeles del font */

	/* Recorrer cada columna del font de la letra c
	 * y por cada columna debe recorrer los 8 bits de la columna.
	 * Si el bit es igual a 1 entonces debe dibujar el bit en el buffer
	 * usando draw_pixel_on_buffer.
	 */

	const char *fp = &font[5 * c];

        for (i = 0; i < 5; i++) {

                col = *(fp + i);

                for (j = 0; j < 7; j++) {

                        if (check_bit(col, j))
                                gui_pixel(x + i, y + j, color);
                        else {
                                if (bg_color) {
                                        gui_pixel(x + i, y + j, bg_color);
                                }
                        }
                }
        }
}

/* Muestra un texto en pantalla
 * x e y son coordenadas a resolución de pixel
 * Cada letra es de 6 columnas y 8 filas (1 columna es espacio)
 */
void gui_print_text(int x, int y, char *text, uint32 color, uint32 bg_color)
{
	const int offset = 6;
	char *c = text;

	/* Recorra el mensaje (puntero c) y dibuje cada letra en el
	 * buffer utilizando la funcion draw_char_on_buffer()
	 *
	 * Importante: cada letra ocupa 5 columnas, por lo que
	 * cada letra debe ir ubicada 6 columnas mas adelante con
	 * respecto a la letra anterior (5 columnas + 1 espacio extra)
	 */
	while (*c) {
		gui_draw_char(x, y, *c, color, bg_color);
		x = x + offset;
		c++;
	}
}

void gui_draw_image(int x, int y, int w, int h, uint32 *image)
{
	// Each image[] position is a pixel color
	int clr_pos = 0;

	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			// Draw using each pixel color (if not transparent)
			if(((image[clr_pos] >> 24) & 0xff) != 0xff)
				gui_pixel(x + j, y + i, image[clr_pos]);
			clr_pos++;
		}
	}
}

void gui_draw_rect(int x, int y, int w, int h, uint32 color)
{
	int clamped_w = ((x + w) > gui_width) ? w - (x + w - gui_width) : w;
	int clamped_h = ((y + h) > gui_height) ? h - (y + h - gui_height) : h;
	for (int j = 0; j < clamped_h; j++)
		for (int i = 0; i < clamped_w; i++)
			gui_buf[(y+j) * gui_width + x + i] = color;
}

void gui_draw_hollow_rect(int x, int y, int w, int h, uint32 color)
{
	for (int i = 0; i < w; i++) {
		gui_pixel(x + i, y, color);
		gui_pixel(x + i, y + (h - 1), color);
	}

	for (int j = 0; j < h; j++) {
		gui_pixel(x, y + j, color);
		gui_pixel(x + (w - 1), y + j, color);
	}
}

void gui_flush(void)
{
	seek(VGA, 0);
	write(VGA, (char *)gui_buf, gui_buf_size);
}

void gui_init(void)
{
	control(VGA, VGA_GET_WIDTH, (int32) &gui_width, NULL);
	control(VGA, VGA_GET_HEIGHT, (int32) &gui_height, NULL);
	control(VGA, VGA_GET_BPP, (int32) &gui_bpp, NULL);
	gui_buf_size = gui_width * gui_height * (gui_bpp/8);
	gui_buf = (uint32 *)getmem(gui_buf_size);
}
