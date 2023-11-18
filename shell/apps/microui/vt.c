#include <xinu.h>
#include <microui.h>
#include <gui_buf.h>
#include <vt100.h>

#define VT_W (80*VT100_CHAR_WIDTH)
#define VT_H (24*VT100_CHAR_HEIGHT)

#define FRAME_SPACE_W 10
#define FRAME_SPACE_H 35


void null_str(char *str) {

}

void vtty_out_set_pid(int n, int pid)
{
	struct ttycblk *typtr;
	typtr = &ttytab[n+1];
	typtr->vtty_out_pid = pid;
}

void vtty_in_set_ch(int n, char ch)
{
	struct ttycblk *typtr;
	typtr = &ttytab[n+1];
	typtr->vtty_in_ch = ch;
	
}

// process vtty_out(int n, struct vt100 *term) 
process vtty_out(int n, int t) 
{
        intmask mask;                   /* Saved interrupt mask         */
	struct ttycblk *typtr;

        int32   ochars;                 /* Number of output chars sent  */
                                        /*   to the UART                */
        int32   avail;                  /* Available chars in output buf*/
        int32   uspace;                 /* Space left in onboard UART   */
	int m;

	typtr = &ttytab[n+1];
	struct vt100 *term = vt100_get_vt(n);
	
while(1) {
	sleepms(10);
	// REMOVE SOON 	m = receive();
	// REMOVE SOON 	mask = disable();
        byte    ier = 0;

        /* If output is currently held, simply ignore the call */

        if (typtr->tyoheld) {
	// REMOVE SOON 		restore(mask);
                continue;
		//  REMOVE SOON return;
        }

        /* If echo and output queues empty, turn off interrupts */

        if ( (typtr->tyehead == typtr->tyetail) &&
             (semcount(typtr->tyosem) >= TY_OBUFLEN) ) {
		// REMOVE SOON	restore(mask);
		continue;
                // REMOVE SOON return;
        }
        
        /* Initialize uspace to the size of the transmit FIFO */

        // NO BUFFER uspace = UART_FIFO_SIZE;

        /* While onboard FIFO is not full and the echo queue is */
        /*   nonempty, xmit chars from the echo queue           */

        while (typtr->tyehead != typtr->tyetail) {
                vt100_putc(term, (char) *typtr->tyehead++);
                if (typtr->tyehead >= &typtr->tyebuff[TY_EBUFLEN]) {
                        typtr->tyehead = typtr->tyebuff;
                }
                // NO BUFFER uspace--;
        }

        /* While onboard FIFO is not full and the output queue is       */
        /*   nonempty, transmit chars from the output queue             */

        ochars = 0;
        avail = TY_OBUFLEN - semcount(typtr->tyosem);
        while ( (avail > 0) ) {
                vt100_putc(term, (char) *typtr->tyohead++);
                if (typtr->tyohead >= &typtr->tyobuff[TY_OBUFLEN]) {
                        typtr->tyohead = typtr->tyobuff;

      }
                avail--;
                // NO BUFFER uspace--;
                ochars++;
        }
        if (ochars > 0) {
                signaln(typtr->tyosem, ochars);
        }

	// REMOVE SOON	restore(mask);
}

}


/*
void test_colors(uint32 *buf, int width_buf)
{
        char b[32];
        
        // reset terminal and clear screen. Cursor at 1;1.
        // reset all modes
        vt100_puts(buf, width_buf, "\e[c\e[2J\e[m\e[r\e[?6l\e[1;1H");

        // set top margin 3 lines, bottom margin 5 lines
        vt100_puts(buf, width_buf, "\e[4;36r");

        // set bg color to red and text to white
        vt100_puts(buf, width_buf, "\e[41;37m");
        
        // draw top and bottom windows
        vt100_puts(buf, width_buf, "\e[1;1H#\e[2;1H#\e[3;1H#\e[1;40H#\e[2;40H#\e[3;40H#");
        vt100_puts(buf, width_buf, "\e[1;1H"); for(int x = 0; x < VT100_WIDTH; x++) vt100_putc(buf, width_buf, '#'); 
        vt100_puts(buf, width_buf, "\e[3;1H"); for(int x = 0; x < VT100_WIDTH; x++) vt100_putc(buf, width_buf, '#');

        // background blue
        vt100_puts(buf, width_buf, "\e[44;37m");
        
        vt100_puts(buf, width_buf, "\e[36;1H#\e[37;1H#\e[38;1H#\e[39;1H#\e[40;1H#");
        vt100_puts(buf, width_buf, "\e[36;40H#\e[37;40H#\e[38;40H#\e[39;40H#\e[40;40H#");
        vt100_puts(buf, width_buf, "\e[36;1H"); for(int x = 0; x < VT100_WIDTH; x++) vt100_putc(buf, width_buf, '#'); 
        vt100_puts(buf, width_buf, "\e[40;1H"); for(int x = 0; x < VT100_WIDTH; x++) vt100_putc(buf, width_buf, '#');

        // foreground white
        vt100_puts(buf, width_buf, "\e[37;40m");


        // print some text that should not move
        vt100_puts(buf, width_buf, "\e[2;4HThis is top text (should not move)"); 
        vt100_puts(buf, width_buf, "\e[38;3HThis is bottom text (should not move)");

        // green background, black text
        vt100_puts(buf, width_buf, "\e[42;30m");
        
        // set origin mode and print border around the scroll region
        vt100_puts(buf, width_buf, "\e[?6h");
        vt100_puts(buf, width_buf, "\e[1;1H"); for(int x = 0; x < VT100_WIDTH; x++) vt100_putc(buf, width_buf, '!');
        // origin mode should snap 99 to last line in scroll region
        vt100_puts(buf, width_buf, "\e[99;1H"); for(int x = 0; x < VT100_WIDTH; x++) vt100_putc(buf, width_buf, '!');
	int l, c;
                sprintf(b, "\e[%d;1H", 1);
                vt100_puts(buf, width_buf, b);
 		for(l = 0; l < 40; l++){
                        for(c = 0; c < 40; c++){
                                vt100_putc(buf, width_buf, '0'+l);
                        }
                	sprintf(b, "\e[%d;1H", l+1);
                	vt100_puts(buf, width_buf, b);
                }
        // scroll the scroll region
        vt100_puts(buf, width_buf, "\e[99;1H\eD\eD");
        vt100_puts(buf, width_buf, "\e[1;1H\eM\eM");
        vt100_puts(buf, width_buf, "\e[99;1H\eD");

        // black background, yellow text
        vt100_puts(buf, width_buf, "\e[33;40m");
        
        // clear out an area in the middle and draw text
        for(int y = 0; y < 5; y++){
                sprintf(b, "\e[%d;6H", y+10);
                vt100_puts(buf, width_buf, b);
        
                for(int c = 0; c < 30; c++){
                        vt100_putc(buf, width_buf, ' ');
                }
        }

        vt100_puts(buf, width_buf, "\e[11;10HMust be ! filled with 2");
        vt100_puts(buf, width_buf, "\e[12;10H    empty lines at");
        vt100_puts(buf, width_buf, "\e[13;10H    top and bottom! ");

}

void test_scroll(uint32 *buf, int width_buf){
        char b[16]; 
        // reset terminal and clear screen. Cursor at 1;1. 
        vt100_puts(buf, width_buf, "\e[c\e[2J\e[m\e[r\e[?6l\e[1;1H");

        // set top margin 3 lines, bottom margin 5 lines
        vt100_puts(buf, width_buf, "\e[4;36r");

        // draw top and bottom windows
        vt100_puts(buf, width_buf, "\e[1;1H#\e[2;1H#\e[3;1H#\e[1;40H#\e[2;40H#\e[3;40H#");
        vt100_puts(buf, width_buf, "\e[36;1H#\e[37;1H#\e[38;1H#\e[39;1H#\e[40;1H#");
        vt100_puts(buf, width_buf, "\e[36;40H#\e[37;40H#\e[38;40H#\e[39;40H#\e[40;40H#");
        vt100_puts(buf, width_buf, "\e[1;1H"); for(int x = 0; x < VT100_WIDTH; x++) vt100_putc(buf, width_buf, '#'); 
        vt100_puts(buf, width_buf, "\e[3;1H"); for(int x = 0; x < VT100_WIDTH; x++) vt100_putc(buf, width_buf, '#'); 
        vt100_puts(buf, width_buf, "\e[36;1H"); for(int x = 0; x < VT100_WIDTH; x++) vt100_putc(buf, width_buf, '#'); 
        vt100_puts(buf, width_buf, "\e[40;1H"); for(int x = 0; x < VT100_WIDTH; x++) vt100_putc(buf, width_buf, '#');

        // print some text that should not move
        vt100_puts(buf, width_buf, "\e[2;4HThis is top text (should not move)"); 
        vt100_puts(buf, width_buf, "\e[38;3HThis is bottom text (should not move)");
        
        // set origin mode and print border around the scroll region
        vt100_puts(buf, width_buf, "\e[?6h");
        vt100_puts(buf, width_buf, "\e[1;1H"); for(int x = 0; x < VT100_WIDTH; x++) vt100_putc(buf, width_buf, '!');
        // origin mode should snap 99 to last line in scroll region
        vt100_puts(buf, width_buf, "\e[99;1H"); for(int x = 0; x < VT100_WIDTH; x++) vt100_putc(buf, width_buf, '!');
        for(int y = 0; y < VT100_HEIGHT; y++){
                //sprintf(buf, "\e[%d;1H!\e[%d;%dH!", y+1, y+1, VT100_WIDTH);
                sprintf(b, "\e[%d;1H", y+1);
                vt100_puts(buf, width_buf, b);
                for(int c = 0; c < VT100_WIDTH; c++){
                        vt100_putc(buf, width_buf, '!');
                }
        }

        // scroll the scroll region
        vt100_puts(buf, width_buf, "\e[99;1H\eD\eD");
        vt100_puts(buf, width_buf, "\e[1;1H\eM\eM");
        vt100_puts(buf, width_buf, "\e[99;1H\eD");

        // clear out an area in the middle and draw text
        for(int y = 0; y < 5; y++){
                sprintf(b, "\e[%d;6H", y+10);
                vt100_puts(buf, width_buf, b);
        
                for(int c = 0; c < 30; c++){
                        vt100_putc(buf, width_buf, ' ');
                }
        }
        vt100_puts(buf, width_buf, "\e[11;10HMust be ! filled with 2");
        vt100_puts(buf, width_buf, "\e[12;10H    empty lines at");
        vt100_puts(buf, width_buf, "\e[13;10H    top and bottom! ");
        
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

*/


process vt(void)
{
	int n, n_vt;
	uint32 *buf;
	struct vt100 *t;
	char title[30];

	buf = gui_buf_getmem(VT_W*VT_H*4);
	n_vt = vt100_get_vt_available(buf, VT_W);
	if (n_vt == -1) {
		gui_buf_freemem(buf, VT_W*VT_H*4);
		return;
	}

	t = vt100_get_vt(n_vt);

	sprintf(title, "virtual terminal %d", n_vt);
	n = mu_add_win(title, 100+n_vt*40, 400-n_vt*40, VT_W, VT_H, buf);
	
	vt100_init(t, null_str);

	/* clear screen */
	vt100_puts(t, "\e[c\e[2J\e[m\e[r\e[?6l\e[1;1H");

	int vtty_pid;
	vtty_pid = create(vtty_out, 2048, 60, "vtty_out", 1, n_vt, t);
	vtty_out_set_pid(n_vt, vtty_pid);

	resume(vtty_pid);

	struct ttycblk *typtr;
	typtr = &ttytab[n_vt+1];
        intmask mask;                   /* Saved interrupt mask         */

        /* Wait for shell to exit and recreate it */
	sleepms(5);
        int shell_pid = create(shell, 4096, 20, "shell", 1, VTTY0+n_vt);
        resume(shell_pid);

	mu_event_t e;
	for (;;) {
		mu_get_event(n, &e);
                if (e.but != -1) {
                        // DO SOMETHING WITH MOUSE printf("mouse x: %d, y: %d \n", e.mouse.x, e.mouse.y);
		}

		if (windows[n].cnt->open == 0) { // esc: TODO replace with enum of some kind
			break;
		}

                if (e.c[0] != '\0') {
			mask = disable();
			typtr->vtty_in_ch = e.c[0];
			ttyhandle_in(typtr, NULL);
			restore(mask);
		}

		sleepms(2);
	};

	kill(vtty_pid);
	kill(shell_pid);
	vt100_free_vt(n_vt);
	gui_buf_freemem(buf, VT_W*VT_H*4);
	mu_free_win(n) ;

}

