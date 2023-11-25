#include <xinu.h>

#include <gui_buf.h>
#include <microui.h>
#include "myLib2.h"
#include "sprites.h"
#include "start2.h"
#include "gameover2.h"

enum GBAState {
	START2,
	START2_NODRAW,
	GAME,
	GAMEOVER
};

int arkanoid(uint32* buf, int w, int n)
{
	
	enum GBAState state = START2;
	enum GBAState prevState = START2;

	MOVOBJ objs;
	MOVOBJ ball;
	MOVOBJ oldobjs;
	MOVOBJ oldball;
	MOVOBJ *cur;
	int oldscore = score;

	char buffer[25];
	int lives = 3;
	int oldlives = lives;
	int currentLevel = 0;

	objs.cd =  0;
	objs.rd = 0;
	objs.width = 18;
	objs.height = 9;
	objs.row = 160-objs.height;
	objs.col = 120-objs.width/2;
	oldobjs = objs;
	
	ball.row = 100;
	ball.col = 100;
	ball.width = 5;
	ball.height = 5;
	ball.cd = 2;
	ball.rd = -2;
	int ballReleased = 1;
	oldball = ball;	

	   mu_event_t e;
	while(1)
	{	
                mu_get_event(n, &e);
		switch(state) {
		case START2: 
			prevState = state;
			state = START2_NODRAW;
			waitForVblank();
			sprintf(buffer, "Press A to Start");
			gui_buf_print_text(buf, w, 55, 50, buffer, WHITE, BLACK);
        	        if (e.c[0] == 'a')
//			if(KEY_DOWN_NOW(BUTTON_SELECT)) //Select back to start
			{
				state = START2;
				break;
			}
			allocateRects();
			break;

		case START2_NODRAW: //Press A on Start or GameOver screen to go to game
        	        if (e.c[0] == 'b')
		//	if(KEY_DOWN_NOW(BUTTON_SELECT))
			{
				state = START2;
				break;
			}
        	        if (e.c[0] == 'c')
		//	if(KEY_DOWN_NOW(BUTTON_A)) // Set up Game
			{
				gui_buf_rect(buf, w, 0,0,160,240,bgcolor);
				lives = 3;
				objs.row = 160-objs.height;
				objs.col = 120-objs.width/2;
				ball.row = 120;
				ball.col = 120;
				score = 0;
				prevState = state;
				state = GAME;
				currentLevel = 0;
				rectangles = createLevel(currentLevel);
			}
			break;

		case GAME:	//Game screen and game mechanics
        	        if (e.c[0] == 'k')
			//if(KEY_DOWN_NOW(BUTTON_RIGHT))
			{
				objs.cd=3;
			}
        	        if (e.c[0] == 'j')
			//if(KEY_DOWN_NOW(BUTTON_LEFT))
			{
				objs.cd=-3;
			}

			if(ballReleased)
			{
				ball.row = objs.row - ball.height;
				ball.col = objs.col + (objs.width/2) - (ball.width/2);
        	        	if (e.c[0] == 'n')
				//if(KEY_DOWN_NOW(BUTTON_B))
				{
					ballReleased = 0;
				}
			} else { //move ball
				cur = &ball;

				cur->row += cur->rd;
				cur->col += cur->cd;
				
				if(cur->row < 0)
				{
					cur->row = 0;
					cur->rd =-cur->rd;
				}
				if(cur->col < 0)
				{
					cur->col = 0;
					cur->cd =-cur->cd;
				}
				if(cur->col > 239-cur->width)
				{
					cur->col = 239-cur->width;
					cur->cd = -cur->cd;
				}
				if(cur->row > 159-cur->height)
				{
					cur->row = 159-cur->height;
					cur->rd = -cur->rd;
					oldlives = lives;
					lives--;
					ballReleased = 1;
				}
				//detect ball and bar collision
				int result = detectCollision(&ball, &objs);
				if(result)
				{
					ball.rd *= -1;
					if(result == 2)
					{
						ball.cd *= -1;
					}
				}
			}
			//set bounds for bar
			cur = &objs;

			cur->col += cur->cd;
			
			if(objs.col>240-objs.width-2) objs.col = 240-objs.width;
			if(objs.col<2) objs.col=0;	


			//detect ball and brick collision and add to score
			oldscore = score;
			for(int i = 0; i < levelSize; i++)
			{
				if(rectangles[i].num != -1)
				{
					if(detectCollisionRect(&ball, rectangles+i))
					{
						ball.rd *= -1;
					}
				}

			}

			//check if all bricks are gone
			int finished = 0;
			for(int i = 0; i < levelSize; i++)
			{
				if(rectangles[i].num != -1)
				{
					finished = 0;
					break;
				}
				finished = 1;
			}

			//all drawing
			waitForVblank();
			//go to game over is <= 0 lives
			if(lives <= 0)
			{
				prevState = state;
				state = GAMEOVER;
				break;
			}
        	        	else if (e.c[0] == 'r')
			// else if(KEY_DOWN_NOW(BUTTON_SELECT))
			{
				lives = 3;
				prevState = state;
				state = START2;
				break;
			}
			//draw next level
			else if(finished)
			{
				gui_buf_rect(buf, w, 0,0,160,240,bgcolor);
				objs.row = 160-objs.height;
				objs.col = 120-objs.width/2;
				ball.row = 120;
				ball.col = 120;
				ballReleased = 1;
				currentLevel++;
				if(currentLevel > 5)
				{
					currentLevel = 0;	
				}
				rectangles = createLevel(currentLevel);
				finished = 0;
			}
			//rest of drawing
			else 
			{

				sprintf(buffer, "Lives: ");
				gui_buf_print_text(buf, w, 0, 25, buffer, YELLOW, BLACK);
				sprintf(buffer, "%d", oldlives);
				gui_buf_print_text(buf, w, 0, 72, buffer, bgcolor, BLACK);
				sprintf(buffer, "%d", lives);
				gui_buf_print_text(buf, w, 0, 72, buffer, YELLOW, BLACK);

				sprintf(buffer, "Score: ");
				gui_buf_print_text(buf, w, 0, 140, buffer, YELLOW, BLACK);
				sprintf(buffer, "%d", oldscore);
				gui_buf_print_text(buf, w, 0, 182, buffer, bgcolor, BLACK);
				sprintf(buffer, "%d", score);
				gui_buf_print_text(buf, w, 0, 182, buffer, YELLOW, BLACK);

				drawLevel(buf, w);

				gui_buf_rect(buf, w, oldobjs.row, oldobjs.col, oldobjs.height, oldobjs.width, bgcolor);
				gui_buf_rect(buf, w, oldball.row, oldball.col, oldball.height, oldball.width, bgcolor);
				
				//drawSpriteImage(rectangle.row, rectangle.col, rectangle.height, rectangle.width, SPRITES_WIDTH, &sprites[OFFSET(83,30,SPRITES_WIDTH)]);
				drawSpriteImage(buf, w, ball.row, ball.col, ball.height, ball.width, SPRITES_WIDTH, &sprites[OFFSET(94,46,SPRITES_WIDTH)]);
				drawSpriteImage(buf, w, objs.row, objs.col, objs.height, objs.width, SPRITES_WIDTH, &sprites[OFFSET(95,60,SPRITES_WIDTH)]);

				//RECT rectangles[] = drawLevel(0);
				//memcpy(rectangles, drawLevel(0), sizeof(dra));

				oldobjs = objs;
				oldball = ball;
				objs.cd=0;
			}
			break;

		//Game over screen
		case GAMEOVER:
			prevState = state;
			state = START2_NODRAW;
			waitForVblank();
			gui_buf_draw_image(buf, w, 0,0,240,160,gameover2);
			sprintf(buffer, "Game Over");
			gui_buf_print_text(buf, w, 80, 60, buffer, WHITE, BLACK);
			sprintf(buffer, "Score: %d", score);
			gui_buf_print_text(buf, w, 89, 60, buffer, WHITE, BLACK);
			lives = 3;
			objs.row = 160-objs.height;
			objs.col = 120-objs.width/2;
			ball.row = 100;
			ball.col = 100;
			score = 0;
			break;	
		}	
	}
	return 0;
}
