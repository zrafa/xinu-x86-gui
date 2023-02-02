#include <xinu.h>
//#include <stdlib.h>
//#include "stdio.h"
#include "myLib.h"
#include "text.h"
#include "sprites.h"
#include "start.h"
//#include <string.h>
#include "gameover.h"

enum GBAState {
	START,
	START_NODRAW,
	GAME,
	GAMEOVER
};

int arkanoid()
{
	// REG_DISPCNT = MODE3 | BG2_ENABLE;
	
	enum GBAState state = START;
	enum GBAState prevState = START;

	//Set up moving objects and properties
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

	while(1)
	{	
		//Switch for different screens
		switch(state) {
		case START: // Start screen
			prevState = state;
			state = START_NODRAW;
			waitForVblank();
			//drawImage3(0,0,240,160,start);
			sprintf(buffer, "Press A to Start");
			drawString(55, 50, buffer, WHITE);
			if(KEY_DOWN_NOW(BUTTON_SELECT)) //Select back to start
			{
				state = START;
				break;
			}
			allocateRects();
			break;

		case START_NODRAW: //Press A on Start or GameOver screen to go to game
			if(KEY_DOWN_NOW(BUTTON_SELECT))
			{
				state = START;
				break;
			}
			if(KEY_DOWN_NOW(BUTTON_A)) // Set up Game
			{
				drawRect(0,0,160,240,bgcolor);
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
			//move bar
			if(KEY_DOWN_NOW(BUTTON_RIGHT))
			{
				objs.cd=3;
			}
			if(KEY_DOWN_NOW(BUTTON_LEFT))
			{
				objs.cd=-3;
			}

			//release ball from bar
			if(ballReleased)
			{
				ball.row = objs.row - ball.height;
				ball.col = objs.col + (objs.width/2) - (ball.width/2);
				if(KEY_DOWN_NOW(BUTTON_B))
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
			else if(KEY_DOWN_NOW(BUTTON_SELECT))
			{
				lives = 3;
				prevState = state;
				state = START;
				break;
			}
			//draw next level
			else if(finished)
			{
				drawRect(0,0,160,240,bgcolor);
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
				drawString(0, 25, buffer, YELLOW);
				sprintf(buffer, "%d", oldlives);
				drawString(0, 72, buffer, bgcolor);
				sprintf(buffer, "%d", lives);
				drawString(0, 72, buffer, YELLOW);

				sprintf(buffer, "Score: ");
				drawString(0, 140, buffer, YELLOW);
				sprintf(buffer, "%d", oldscore);
				drawString(0, 182, buffer, bgcolor);
				sprintf(buffer, "%d", score);
				drawString(0, 182, buffer, YELLOW);

				drawLevel();

				drawRect(oldobjs.row, oldobjs.col, oldobjs.height, oldobjs.width, bgcolor);
				drawRect(oldball.row, oldball.col, oldball.height, oldball.width, bgcolor);
				
				//drawSpriteImage(rectangle.row, rectangle.col, rectangle.height, rectangle.width, SPRITES_WIDTH, &sprites[OFFSET(83,30,SPRITES_WIDTH)]);
				drawSpriteImage(ball.row, ball.col, ball.height, ball.width, SPRITES_WIDTH, &sprites[OFFSET(94,46,SPRITES_WIDTH)]);
				drawSpriteImage(objs.row, objs.col, objs.height, objs.width, SPRITES_WIDTH, &sprites[OFFSET(95,60,SPRITES_WIDTH)]);

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
			state = START_NODRAW;
			waitForVblank();
			drawImage3(0,0,240,160,gameover);
			sprintf(buffer, "Game Over");
			drawString(80, 60, buffer, WHITE);
			sprintf(buffer, "Score: %d", score);
			drawString(89, 60, buffer, WHITE);
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
