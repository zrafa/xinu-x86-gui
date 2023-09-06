#include <xinu.h>
#include <font.h>


uint32 * gui_buf_getmem(int size)
{
	uint32 *b;
	b = getmem(size);
	return b;
}

void gui_buf_freemem(uint32 *buf, int size)
{
	freemem(buf, size);
}

void gui_buf_pixel(uint32 *buf, int w, int x, int y, uint32 color)
{

	buf[y * w + x] = color;
}

void gui_buf_rect(uint32 *buf, int width_buf, int x, int y, int width, int heigth, uint32 color)
{
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < heigth; j++) {
			gui_buf_pixel(buf, width_buf, x + i, y + j, color);
		}
	}
}

void gui_buf_hollow_rect(uint32 *buf, int width_buf, int x, int y, int width, int height, uint32 color)
{
	for (int i = 0; i < width; i++) {
		gui_buf_pixel(buf, width_buf, x + i, y, color);
		gui_buf_pixel(buf, width_buf, x + i, y + (height - 1), color);
	}
	for (int j = 0; j < height; j++) {
		gui_buf_pixel(buf, width_buf, x, y + j, color);
		gui_buf_pixel(buf, width_buf, x + (width - 1), y + j, color);
	}
}

void gui_buf_draw_image(uint32 *buf, int width_buf, int x, int y, int width, int height, uint32 *image)
{
        for (int i = 0; i < width; i++) {
                for (int j = 0; j < height; j++) {
                        gui_buf_pixel(buf, width_buf, x + i, y + j,  *(image + i + (width * j)) );
                }
        }
}


void gui_buf_draw_char(uint32 *buf, int width_buf, int x, int y, char c, uint32 color,
                                   uint32 bg_color)
{

        int i;    /* para recorrer el font por columnas */
        int j;    /* para recorrer el font por filas */
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
                                gui_buf_pixel(buf, width_buf, x + i, y + j, color);
                        else {
                                if (bg_color) {
                                        gui_buf_pixel(buf, width_buf, x + i, y + j, bg_color);
                                }
                        }
                }
        }
}

void gui_buf_print_text(uint32 *buf, int width_buf, int x, int y, char *text, uint32 color, uint32 bg_color)
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
                gui_buf_draw_char(buf, width_buf, x, y, *c, color, bg_color);
                x = x + offset;
                c++;
        }
}
