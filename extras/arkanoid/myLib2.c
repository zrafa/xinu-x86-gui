
#include <xinu.h>
#include <gui_buf.h>
#include "myLib2.h"
#include "sprites.h"


//level design
unsigned char levels[][75] = {
	{9,9,9, 9,9,9, 9,9,9, 9,9,9, 9,9,9, 5,5,5, 5,5,5, 5,5,5, 5,5,5, 5,5,5, 2,2,2, 2,2,2, 2,2,2, 2,2,2, 2,2,2,
		6,6,6, 6,6,6, 6,6,6, 6,6,6, 6,6,6, 7,7,7, 7,7,7, 7,7,7, 7,7,7, 7,7,7},
	{5,5,5, 5,5,5, 5,5,5, 5,5,5, 5,5,5, 1,1,1, 1,1,1, 1,1,1, 1,1,1, 1,1,1, 2,2,2, 2,2,2, 2,2,2, 2,2,2, 2,2,2,
		8,8,8, 8,8,8, 8,8,8, 8,8,8, 8,8,8, 7,7,7, 7,7,7, 7,7,7, 7,7,7, 7,7,7},
	{7,7,7, 7,7,7, 7,7,7, 7,7,7, 7,7,7, 2,2,2, 2,2,2, 2,2,2, 2,2,2, 2,2,2, 5,5,5, 5,5,5, 5,5,5, 5,5,5, 5,5,5, 
		1,1,1, 1,1,1, 1,1,1, 1,1,1, 1,1,1, 8,8,8, 8,8,8, 8,8,8, 8,8,8, 8,8,8},
	{1,1,1, 1,1,1, 1,1,1, 1,1,1, 1,1,1, 5,5,5, 5,5,5, 5,5,5, 5,5,5, 5,5,5, 2,2,2, 2,2,2, 2,2,2, 2,2,2, 2,2,2,
		6,6,6, 6,6,6, 6,6,6, 6,6,6, 6,6,6, 7,7,7, 7,7,7, 7,7,7, 7,7,7, 7,7,7},
	{8,8,8, 8,8,8, 8,8,8, 8,8,8, 8,8,8, 5,5,5, 5,5,5, 5,5,5, 5,5,5, 5,5,5, 6,6,6, 6,6,6, 6,6,6, 6,6,6, 6,6,6, 
		2,2,2, 2,2,2, 2,2,2, 2,2,2, 2,2,2, 7,7,7, 7,7,7, 7,7,7, 7,7,7, 7,7,7}	
};

struct RECT_S RECT_default = {.row = 0, .col = 0, .width = 15, .height = 7, .img = -1, .num=-1, .deleted=0};

RECT *rectangles = NULL;
int levelSize;
//int bgcolor = 0x2100;
int bgcolor = 0x00;
int score = 0;


void waitForVblank()
{
//	while(SCANLINECOUNTER > 160);
//	while(SCANLINECOUNTER < 160);
	sleepms(5);
}


void drawSpriteImage(uint32* buf, int w, int y, int x, int height, int width, int imageWidth, const int* image) {


	for (int i = 0; i < width; i++) {
                for (int j = 0; j < height; j++) {
			uint32 color = (unsigned short *)(image + i + (width * j));
                        gui_buf_pixel(buf, w, x + i, y + j, color );
                }
        }

}


void allocateRects()
{
	printf("mem pedida: %d  \n", 100*sizeof(RECT));
	rectangles = getmem(100 * sizeof(RECT));
}

//Create the level
RECT* createLevel(short level)
{
	levelSize = sizeof(levels[level])/sizeof(char);
	unsigned char levelFormat[levelSize];
	memcpy(levelFormat,levels[level],levelSize);
	
	for(unsigned int i = 0; i < sizeof(levelFormat)/(sizeof(char)*15); i++)
	{
		for(int j = 0; j < 15; j++)
		{
			rectangles[i*15+j] = RECT_default;
			rectangles[i*15+j] = (RECT) {.row = (20+i*8), .col = (16*j), .width = 15, .height = 7, .img = OFFSET(83,((levelFormat[i*15+j]-1) * 16 + 30),SPRITES_WIDTH), .num=(levelFormat[i*15+j]-1), .deleted=0};
		}
	}
	return rectangles;
	//return rectangles;
}

//Draw the level
void drawLevel(uint32* buf, int w)
{
	for(int i = 0; i < levelSize/15; i++)
	{
		for(int j = 0; j < 15; j++)
		{
			if(rectangles[i*15+j].num != -1) {
				drawSpriteImage(buf, w, rectangles[i*15+j].row, rectangles[i*15+j].col, rectangles[i*15+j].height, rectangles[i*15+j].width, SPRITES_WIDTH, &sprites[rectangles[i*15+j].img]);
			} else if(!rectangles[i*15+j].deleted) {
				gui_buf_rect(buf, w, rectangles[i*15+j].row, rectangles[i*15+j].col, rectangles[i*15+j].height, rectangles[i*15+j].width, bgcolor);
				rectangles[i*15+j].deleted = 1;
			}
		}
	}
}

//detect bar and ball collision
int detectCollision(MOVOBJ* ball, MOVOBJ* rect)
{
	if(ball->col <= rect->col + rect->width &&
	   ball->col + ball->width >= rect->col &&
	   ball->row <= rect->row + rect->height &&
	   ball->height + ball->row >= rect->row)
	{
		if(ball->col + ball-> width == rect->col)
		{
			return 1;
		}
		else if(ball->col == rect->col + rect->width)
		{
			return 2;
		}
		return 3;
		
	}
	return 0;
}

//detect brick and ball collision
int detectCollisionRect(MOVOBJ *ball, RECT *rect)
{
	if(rect->num == -1)
		return 0;
	if(ball->col < rect->col + rect->width &&
	   ball->col + ball->width > rect->col &&
	   ball->row < rect->row + rect->height &&
	   ball->height + ball->row > rect->row)
	{
		score += (rect->num + 1) * 10;
		rect->num = -1;
		return 1;
		
	}
	return 0;
}


void freeRectangles() 
{
//	free(rectangles);
}



