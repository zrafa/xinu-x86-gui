
#include<xinu.h>
//#include <stdlib.h>
#include "myLib.h"
#include "sprites.h"
// #include <string.h>

unsigned short *videoBuffer = (unsigned short *)0x6000000;

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
u16 bgcolor = 0x2100;
int score = 0;

//void drawRect(int x, int y, int width, int height, volatile u16 color) {
void drawRect(int x, int y, int width, int height, volatile unsigned short color) {
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			setPixel(x + i, y + j, color);
		}
	}
}

/*
void drawRect(int row, int col, int height, int width, volatile unsigned short color)
{
	for(int r=0; r<height; r++)
	{
		REG_DMA3SAD = (u32)&color;
		REG_DMA3DAD = (u32)(&videoBuffer[OFFSET(row+r, col, 240)]);
		REG_DMA3CNT = width | DMA_ON | DMA_SOURCE_FIXED;
	}
}
*/

void waitForVblank()
{
//	while(SCANLINECOUNTER > 160);
//	while(SCANLINECOUNTER < 160);
}

/* drawimage3
* A function that will draw an arbitrary size image
* onto the screen (with DMA).
* @param r row to draw the image
* @param c column to draw the image
* @param width width of the image
* @param height height of the image
* @param image Pointer to the first element of the image
*/
void drawImage3(int x, int y, int width, int height, const u16* image) {
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			u16 color = *(image + i + (width * j));
			setPixel(x + i, y + j, color);
		}
	}
}

/*
void drawImage3(int r, int c, int width, int height, const u16* image)
{
	for(int row = 0; row < height; row++)
	{
		DMA[DMA_CHANNEL_3].src = image + row * width;
		DMA[DMA_CHANNEL_3].dst = videoBuffer + (row+r)*240 + c;
		DMA[DMA_CHANNEL_3].cnt = width | DMA_ON | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT;
	}
}
*/

void drawSpriteImage(int y, int x, int height, int width, int imageWidth, const u16* image) {
/*
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			u16 color = *(image + i + (width * j));
			setPixel(x + i, y + j, color);
		}
	}
*/
}

//draw images from sprite sheet
/*
void drawSpriteImage(int row, int col, int height, int width, int imageWidth, const unsigned short *image) {

	for(int r = 0; r < height; r++)
	{
		DMA[DMA_CHANNEL_3].src = image + r * imageWidth;
		DMA[DMA_CHANNEL_3].dst = videoBuffer + (row+r)*240 + col;
		DMA[DMA_CHANNEL_3].cnt = width | DMA_ON | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT;
	}
}
*/

#define malloc(x)	(void *)getmem(x)
void allocateRects()
{
	printf("mem pedida: %d  \n", 100*sizeof(RECT));
	rectangles = malloc(100 * sizeof(RECT));
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
void drawLevel()
{
	for(int i = 0; i < levelSize/15; i++)
	{
		for(int j = 0; j < 15; j++)
		{
			if(rectangles[i*15+j].num != -1) {
				drawSpriteImage(rectangles[i*15+j].row, rectangles[i*15+j].col, rectangles[i*15+j].height, rectangles[i*15+j].width, SPRITES_WIDTH, &sprites[rectangles[i*15+j].img]);
			} else if(!rectangles[i*15+j].deleted) {
				drawRect(rectangles[i*15+j].row, rectangles[i*15+j].col, rectangles[i*15+j].height, rectangles[i*15+j].width, bgcolor);
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

/*
void setPixel(int row, int col, unsigned short color)
{
	videoBuffer[OFFSET(row, col, 240)] = color;
}
*/

void freeRectangles() 
{
//	free(rectangles);
}



