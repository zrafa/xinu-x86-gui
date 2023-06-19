// /* source: https://forum.osdev.org/viewtopic.php?t=10247 */

#include <xinu.h>
#include <stdio.h>
#include <string.h>
#include <font.h>
#include <vga.h>

uint32 rgb16_to_rgb32(u16 a)
{
    /* 1. Extract the red, green and blue values */

    /* from rrrr rggg gggb bbbb */
    uint32 r = (a & 0xF800) >>11;
    uint32 g = (a & 0x07E0) >>5;
    uint32 b = (a & 0x001F);

    /* 2. Convert them to 0-255 range:
    There is more than one way. You can just shift them left:
    to 00000000 rrrrr000 gggggg00 bbbbb000
    r <<= 3;
    g <<= 2;
    b <<= 3;
    But that means your image will be slightly dark and
    off-colour as white 0xFFFF will convert to F8,FC,F8
    So instead you can scale by multiply and divide: */
    r = r * 255 / 31;
    g = g * 255 / 63;
    b = b * 255 / 31;
    /* This ensures 31/31 converts to 255/255 */

    /* 3. Construct your 32-bit format (this is 0RGB): */
    //return (r << 16) | (g << 8) | b;
    return (b << 16) | (g << 8) | r;


    /* Or for BGR0: */
    //return (r << 8) | (g << 16) | (b << 24);
}

void gui_set_pixel(unsigned x, unsigned y, u16 color) {
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
	uint32 new_pos = y * vga->pitch + x*(vga->bpp/8);
	char *ptr = &buffer;
	open(VGA, 0, 0);
	seek(VGA, new_pos);
        write(VGA, ptr, 4);
    close(VGA);
}


void gui_paint_screen(){
	uint32 total_x = 1024,
		total_y = 768*4,
		actual_x = 0,
		actual_y = 0,
                new_pos;
	

	uint32 buffer[total_x];
	for(uint32 y = actual_y; y < total_y; y++){
		for(int x = 0; x < total_x; x++){
			buffer[x] = 0x00ffff00;
		}
		new_pos = 0 + y * total_x;
		actual_x++;
		open(VGA, 0, 0);
		seek(VGA, new_pos);
		char *ptr = &buffer;
		write(VGA, ptr, total_x*4);
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
void draw_char_on_buffer(unsigned int x, unsigned int y, char c,
                            unsigned char color)
{

        int i; /* para recorrer el font por columnas */
        int j; /* para recorrer el font por filas */
        char col; /* para almacenar cada columna de pixeles del font */


        /* Recorrer cada columna del font de la letra c
         * y por cada columna debe recorrer los 8 bits de la columna. 
         * Si el bit es igual a 1 entonces debe dibujar el bit en el buffer
         * usando draw_pixel_on_buffer.
         */

        const char *fp = &font[5 * c];

        for (i = 0; i < 5; i++) {
                col = *(fp+i);
                for (j = 0; j < 7; j++) {
                        if (check_bit(col, j)) {
                                gui_pixel(x+i, y+j, 0x00000000);
                        } else {
                                gui_pixel(x+i, y+j, 0x00ffff00);
                        }
                }
        }
}

// /* Muestra un texto en pantalla
//  * x e y son coordenadas a resolución de pixel
//  * Cada letra es de 6 columnas y 8 filas (1 columna es espacio)
//  */
void gui_print_text_on_vga(unsigned int x, unsigned int y, char *text)
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
        while (*c) {
                draw_char_on_buffer(x, y, *c, 0);
                x = x + offset;
                c++;
        }
}


