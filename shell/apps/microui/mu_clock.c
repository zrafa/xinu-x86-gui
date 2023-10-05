#include <xinu.h>
#include <shprototypes.h>
#include <mat.h>
#include <microui.h>
#include <gui_buf.h>
#include <clock_image.h>

#define W 160
#define H 120

#define PI  ((double) 3.1415926535)

// Function for finding absolute value 
//int abs(int n) { return ((n > 0) ? n : (n * (-1))); } 
  
// DDA Function for line generation 
void draw_line(uint32 *buf, int w, int X0, int Y0, int X1, int Y1) 
{ 
    // calculate dx & dy 
    int dx = X1 - X0; 
    int dy = Y1 - Y0; 
  
    // calculate steps required for generating pixels 
    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy); 
  
    // calculate increment in x & y for each steps 
    float Xinc = dx / (float)steps; 
    float Yinc = dy / (float)steps; 
  
    // Put pixel for each step 
    float X = X0; 
    float Y = Y0; 
    for (int i = 0; i <= steps; i++) { 
  	gui_buf_pixel(buf, w, (int) X, (int) Y, 0x00);
        //putpixel(round(X), round(Y), 
                 //RED); // put pixel at (X,Y) 
        X += Xinc; // increment in x at each step 
        Y += Yinc; // increment in y at each step 
    } 
} 


void show_aguja(uint32 *buf, int w, double angle, int r)
{
	double x1, y1;

            x1 = r * cos(angle * PI / 180);
            y1 = r * sin(angle * PI / 180);
		draw_line(buf, W, 80, 50, 80+x1, 50+y1);
		draw_line(buf, W, 81, 50, 81+x1, 50+y1);
		draw_line(buf, W, 79, 50, 79+x1, 50+y1);
		draw_line(buf, W, 80, 51, 80+x1, 51+y1);
		draw_line(buf, W, 80, 49, 80+x1, 49+y1);
}

void show_time(uint32 *buf, int w) 
{
	uint32 now;
        char    datestr[64];            /* printable date in ascii      */
	int32 year, days, tmp, month, hour, minute, second;
	double angle;

        gettime(&now);
        ascdate(now, datestr);



	/* hour */

	datestr[15] = 0;
	hour = (double) atoi(&datestr[13]);
	if (hour > 12) hour = hour - 12;
	angle = (360/12) * hour + 270;
	if (angle > 360) angle = angle - 360;
	show_aguja(buf, w, angle, 20);

	datestr[18] = 0;
	minute = (double) atoi(&datestr[16]);
	angle = (360/60) * minute + 270;
	if (angle > 360) angle = angle - 360;
	show_aguja(buf, w, angle, 40);

	datestr[21] = 0;
	second = (double) atoi(&datestr[19]);
	angle = (360/60) * second + 270;
	if (angle > 360) angle = angle - 360;
	show_aguja(buf, w, angle, 45);


	datestr[3] = 0;
	datestr[11] = 0;
	gui_buf_print_text(buf, w, 45, 104, datestr, 0x00, 0x00FFFFFF);
	gui_buf_print_text(buf, w, 45+(6*4), 104, &datestr[4], 0x00, 0x00FFFFFF);
}

process mu_clock(void)
{
	int n;
	uint32 *buf;

	buf = gui_buf_getmem(W*H*4);
	gui_buf_draw_image(buf, W, 0, 0, W, H, pixel_data);
	n = mu_add_win("Date and Time", 800, 600, W, H, buf);

	/* program source code (for example, modify surface
	 * drawn into window
	 */

	int msg;
	int child;
	const char * args = "-f"; 


	child = create(xsh_date, 4096, 30, "date", 2, 1, args);
	msg = recvclr();
        resume(child);
	msg = receive();
	while (msg != child) {
		msg = receive();
	}
	Date.dt_bootvalid = TRUE;



      double i, angle, x1, y1;

	int x = 50; 
	int y = 50; 
	int r = 45; 
	int step = 360 / 60;

      /* draw circle */
	/*
      	 * for(i = 0; i < 360; i += 0.1)
      	 * {
       	 *      angle = i;
      	 *       x1 = r * cos(angle * PI / 180);
       	 *      y1 = r * sin(angle * PI / 180);
       	 *      gui_buf_pixel(buf, W, x + x1, y + y1, 0xFFFFFFFF);
	 * 		sleep(1);
    	 *   }
	*/

	mu_event_t e;
	for (;;) {
		gui_buf_draw_image(buf, W, 0, 0, W, H, pixel_data);
		show_time(buf, W);

		mu_get_event(n, &e);
                if (e.but != -1)
                        printf("mouse x: %d, y: %d \n", e.mouse.x, e.mouse.y);
                if (e.c[0] != '\0')
                        printf("KEY: %c %d \n", e.c[0], e.c[0]);

		sleep(1);
	};

	/* wait until window closes or program finishes */
	sleep(100);	

	gui_buf_freemem(buf, W*H*4);
	mu_free_win(n) ;
}

