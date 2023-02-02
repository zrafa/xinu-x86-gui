/* source: https://forum.osdev.org/viewtopic.php?t=10247 */

#include <xinu.h>
#include <stdio.h>
#include <string.h>
#include <font.h>


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
                                pixel(x+i, y+j, 0x00000000);
                        } else {
                                pixel(x+i, y+j, 0x00ffff00);
                        }
                }
        }
}

/* Muestra un texto en pantalla
 * x e y son coordenadas a resolución de pixel
 * Cada letra es de 6 columnas y 8 filas (1 columna es espacio)
 */
void print_text_on_vga(unsigned int x, unsigned int y, char *text)
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


void borrar(int x1, int y1, int x2, int y2)
{
	int i,j;
	for (i=x1; i<x2;i++)
	for (j=y1; j<y2;j++)
		pixel(i, j, 0x00ffff00);


}


