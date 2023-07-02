// /* source: https://forum.osdev.org/viewtopic.php?t=10247 */

#include <xinu.h>
#include <stdio.h>
#include <string.h>
#include <font.h>

uint32 rgb16_to_rgb32(uint16 color)
{
	/* 1. Extract the red, green and blue values */

	// From rrrr rggg gggb bbbb, get r5, g6 and b5
	uint32 r = (color >> 11) & 0x1F;
	uint32 g = (color >> 5) & 0x3F;
	uint32 b = (color & 0x1F);

	/* 2. Convert them to r8, g8 and b8 (0-255 value), applying rule of three */
	r = (r * 255) / 31;
	g = (g * 255) / 63;
	b = (b * 255) / 31;

	/* 3. Construct 32-bit format, we get 0x00RRGGBB by shifting bits */
	uint32 color32 = 0x00ffffff & ((r << 16) | (g << 8) | b);
	return color32;
}

void gui_set_pixel(unsigned x, unsigned y, uint16 color)
{
	if ((x >= 240) || (y >= 160))
		return;

	uint32 color32 = rgb16_to_rgb32(color);
	gui_pixel(x, y, color32);
}

/*
	The new position is calculated by doing
y*(the total number of pixels in a line) +
x*(the number of bits per pixel divided by 8)
*/
void gui_pixel(unsigned x, unsigned y, uint32 color)
{
	uint32 buffer = color;
	uint32 new_pos = y * vga->pitch + x * (vga->bpp / 8);
	char *ptr = &buffer;
	open(VGA, 0, 0);
	seek(VGA, new_pos);
	write(VGA, ptr, 4);
	close(VGA);
}

void gui_paint_screen()
{
	uint32 total_x = 1024,
		   total_y = 768 * 4,
		   actual_x = 0,
		   actual_y = 0,
		   new_pos;

	uint32 buffer[total_x];
	for (uint32 y = actual_y; y < total_y; y++)
	{
		for (int x = 0; x < total_x; x++)
		{
			buffer[x] = 0x00ffff00;
		}
		new_pos = 0 + y * total_x;
		actual_x++;
		open(VGA, 0, 0);
		seek(VGA, new_pos);
		char *ptr = &buffer;
		write(VGA, ptr, total_x * 4);
		close(VGA);
	}
}

char check_bit(unsigned char c, int pos)
{
	unsigned char is_bit_at_pos_on = c & (1 << pos);

	return is_bit_at_pos_on ? 1 : 0;
}

/* Dibuja una letra en el buffer, comenzando con la columna [x,y]
 * para el primer pixel de la letra
 */
void gui_draw_char(unsigned int x, unsigned int y, char c, uint32 color,
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

	for (i = 0; i < 5; i++)
	{
		col = *(fp + i);
		for (j = 0; j < 7; j++)
		{
			if (check_bit(col, j))
			{
				gui_pixel(x + i, y + j, color);
			}
			else
			{
				gui_pixel(x + i, y + j, bg_color);
			}
		}
	}
}

// /* Muestra un texto en pantalla
//  * x e y son coordenadas a resolución de pixel
//  * Cada letra es de 6 columnas y 8 filas (1 columna es espacio)
//  */
void gui_print_text(unsigned int x, unsigned int y, char *text, uint32 color, uint32 bg_color)
{
	int i = 0;
	const int offset = 6;
	char *c = text;

	/* Recorra el mensaje (puntero c) y dibuje cada letra en el
	 * buffer utilizando la funcion draw_char_on_buffer()
	 *
	 * Importante: cada letra ocupa 5 columnas, por lo que
	 * cada letra debe ir ubicada 6 columnas mas adelante con
	 * respecto a la letra anterior (5 columnas + 1 espacio extra)
	 */
	while (*c)
	{
		gui_draw_char(x, y, *c, color, bg_color);
		x = x + offset;
		c++;
	}
}

void gui_draw_image(int x, int y, int h, int w, const uint32 *image)
{
	// Each image[] position is a pixel color
	int colorPos = 0;
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			// Draw using each pixel color
			gui_pixel(x + j, y + i, image[colorPos]);
			colorPos++;
		}
	}
}

void gui_draw_rect(int x, int y, int w, int h, uint32 color)
{
	for (int i = 0; i < w; i++)
	{
		for (int j = 0; j < h; j++)
		{
			gui_pixel(x + i, y + j, color);
		}
	}
}

void gui_draw_hollow_rect(int x, int y, int w, int h, uint32 color)
{
	for (int i = 0; i < w; i++)
	{
		gui_pixel(x + i, y, color);
		gui_pixel(x + i, y + (h - 1), color);
	}
	for (int j = 0; j < h; j++)
	{
		gui_pixel(x, y + j, color);
		gui_pixel(x + (w - 1), y + j, color);
	}
}