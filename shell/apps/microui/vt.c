#include <xinu.h>
#include <microui.h>
#include <gui_buf.h>
#include <vt100.h>

#define FONT_W 6
#define FONT_H 8

#define VT_W (80*FONT_W)
#define VT_H (24*FONT_H)

#define FRAME_SPACE_W 10
#define FRAME_SPACE_H 35



void null_str(char *str) {

}


void test_cursor(uint32 *buf, int width_buf){
        char b[16]; 
        // clear screen
        vt100_puts(buf, width_buf, "\e[c\e[2J\e[m\e[r\e[?6l\e[1;1H");

        // draw a line of "*"
        for(int c = 0; c < VT100_WIDTH; c++){
                vt100_putc(buf, width_buf, '*'); 
        }
        // draw left and right border
        for(int c = 0; c < VT100_HEIGHT; c++){
                sprintf(b, "\e[%d;1H*\e[%d;%dH*", c + 1, c + 1, VT100_WIDTH);
                vt100_puts(buf, width_buf, b);
        }
        // draw bottom line
        sprintf(b, "\e[%d;1H", VT100_HEIGHT);
        vt100_puts(buf, width_buf, b); 
        for(int c = 0; c < VT100_WIDTH; c++){
                vt100_putc(buf, width_buf, '*');
        }
        // draw inner border of +
        vt100_puts(buf, width_buf, "\e[2;2H");
        // draw a line of "*"
        for(int c = 0; c < VT100_WIDTH - 2; c++){
                vt100_putc(buf, width_buf, '+'); 
        }
        // draw left and right border
        for(int c = 1; c < VT100_HEIGHT - 1; c++){
                sprintf(b, "\e[%d;2H+\e[%d;%dH+", c + 1, c + 1, VT100_WIDTH - 1);
                vt100_puts(buf, width_buf, b);
        }
        // draw bottom line
        sprintf(b, "\e[%d;2H", VT100_HEIGHT - 1);
        vt100_puts(buf, width_buf, b); 
        for(int c = 0; c < VT100_WIDTH - 2; c++){
                vt100_putc(buf, width_buf, '+');
 }

        // draw middle window
        // EEEEEEEEEEEEEEEEEEEEEEEEEEEEEE
        // E                            E
        // E This must be an unbroken a E
        // E rea of text with 1 free bo E
        // E rder around the text.      E
        // E                            E
        // EEEEEEEEEEEEEEEEEEEEEEEEEEEEEE
        vt100_puts(buf, width_buf, "\e[10;6H");
        for(int c = 0; c < 30; c++){
                vt100_putc(buf, width_buf, 'E');
        }
        // test normal movement
        vt100_puts(buf, width_buf, "\e[11;6H");
        // test cursor store and restore...
        vt100_puts(buf, width_buf, "\e7\e[35;10H\e8");
        vt100_puts(buf, width_buf, "E\e[11;35HE");
        // goto 12;6, print E, move cursor 29 (already moved +1) to right and print E
        vt100_puts(buf, width_buf, "\e[12;6HE\e[28CE");
        // move cursor 31 to left, 1 down, print E, move 30 right, print E
        vt100_puts(buf, width_buf, "\e[30D\e[BE\e[28CE");
        vt100_puts(buf, width_buf, "\e[15;6H\e[AE\e[28CE");
        vt100_puts(buf, width_buf, "\e[15;6HE\e[15;35HE"); 
        
        vt100_puts(buf, width_buf, "\e[16;6H");
        for(int c = 0; c < 30; c++){
                vt100_putc(buf, width_buf, 'E');
        }

        const char *text[] = {"This must be an unbroken a", "rea of text with 1 free bo", "rder around the text.     "};
        for(int c = 0; c < 3; c++){
                sprintf(b, "\e[%d;8H", c + 12);
                vt100_puts(buf, width_buf, b);

   vt100_puts(buf, width_buf, text[c]);
        }

        // now lets draw two parallel columns of Es
        vt100_puts(buf, width_buf, "\e[20;19H"); 
        for(int c = 0; c < 10; c++){
                // draw E (cursor moves right), step one right, draw F, step 3 left and 1 down
                vt100_puts(buf, width_buf, "E\e[1CF\e[3D\e[B");
        }
        
        // Test index (escD - down with scroll)
        // Test reverse index (escM)
        // next line (escE) - cr + index
        // save and restore cursor

        // move to last line and scroll down 8 lines
        vt100_puts(buf, width_buf, "\e[40;1H");
        for(int c = 0; c < 7; c++){
                vt100_puts(buf, width_buf, "\eD");
        }
        // now scroll same number of lines back and then back again (to test up scroll)
        vt100_puts(buf, width_buf, "\e[1;1H");
        for(int c = 0; c < 7; c++){
                vt100_puts(buf, width_buf, "\eM");
        }
        vt100_puts(buf, width_buf, "\e[40;1H");
        for(int c = 0; c < 7; c++){
                vt100_puts(buf, width_buf, "\eD");
        }
  // we now have the Es at the third line (or we SHOULD have)
        // refill the top border and clear bottom borders
        for(int c = 1; c < VT100_WIDTH - 1; c++){
                // we print * then move down and left, print + and go back right and top
                // (good way to test cursor navigation keys)
                sprintf(b, "\e[1;%dH*\e[B\e[D+\e[A", c + 1); 
                vt100_puts(buf, width_buf, b);
        }
        // clear the border that scrolled up
        for(int c = 2; c < VT100_WIDTH - 2; c++){
                // space, down, left, space, up
                sprintf(b, "\e[32;%dH \e[B\e[D \e[A", c + 1); 
                vt100_puts(buf, width_buf, b);
        }
        
        // redraw left and right border
        for(int c = 1; c < VT100_HEIGHT; c++){
                sprintf(b, "\e[%d;1H*+\e[%d;%dH+*", c + 1, c + 1, VT100_WIDTH - 1);
                vt100_puts(buf, width_buf, b);
        }
        
        // fill border at the bottom
        for(int c = 1; c < VT100_WIDTH - 1; c++){
                sprintf(buf, "\e[39;%dH+\e[B\e[D*\e[A", c + 1); 
                vt100_puts(buf, width_buf, b);
        }
        // draw the explanation string
        vt100_puts(buf, width_buf, "\e[30;6HShould see two columns of E F"); 
        vt100_puts(buf, width_buf, "\e[31;6HText box must start at line 3"); 
}



process vt(void)
{
	int n;
	uint32 *buf;

	buf = gui_buf_getmem(VT_W*VT_H*4);
        gui_buf_print_text(buf, VT_W, 10, 10, "HOLA MUNDO como estan", 255, 0);
//	buf = getmem(NINA_W*NINA_H*4);
//	memcpy(buf, nina, NINA_W*NINA_H*4);
	n = mu_add_win("vt", 700, 240, VT_W, VT_H, buf);
	
	vt100_init(null_str);

	/* program source code (for example, modify surface
	 * drawn into window
	 */

	int l, c;
 		for(l = 0; l < 40; l++){
                        for(c = 0; c < 40; c++){
                                vt100_putc(buf, VT_W, '0'+l);
                        }
                        if(l < 39) vt100_puts(buf, VT_W, "\r\n"); 
                }
	printf ("PASAMOS\n");
  vt100_puts(buf, VT_W, "\e[c\e[2J\e[m\e[r\e[?6l\e[1;1H");
	test_cursor(buf, VT_W);

	mu_event_t e;
	for (;;) {
		mu_get_event(n, &e);
                if (e.but != -1)
                        printf("mouse x: %d, y: %d \n", e.mouse.x, e.mouse.y);
                if (e.c[0] != '\0')
                        printf("KEY: %c %d \n", e.c[0], e.c[0]);

	};

	/* wait until window closes or program finishes */
	sleep(100);	

	gui_buf_freemem(buf, VT_W*VT_H*4);
	mu_free_win(n) ;
}

