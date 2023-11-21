#ifndef MYLIB2_H
#define MYLIB2_H

#define NUMOBJS 7

#define OFFSET(row, col, rowlen)  ((row)*(rowlen)+(col))

#define COLOR(r, g, b) ((r) | (g)<<5 | (b)<<10)
#define WHITE COLOR(31,31,31)
#define RED COLOR(31,0,0)
#define GREEN COLOR(0,31,0)
#define BLUE COLOR(0,0,31)
#define MAGENTA COLOR(31, 0, 31)
#define CYAN COLOR(0, 31, 31)
#define YELLOW COLOR(31, 31, 0)
#define GREY COLOR(25, 25, 25)
#define BLACK 0

// Buttons

#define BUTTON_A		(1<<0)
#define BUTTON_B		(1<<1)
#define BUTTON_SELECT	(1<<2)
#define BUTTON_START	(1<<3)
#define BUTTON_RIGHT	(1<<4)
#define BUTTON_LEFT		(1<<5)
#define BUTTON_UP		(1<<6)
#define BUTTON_DOWN		(1<<7)
#define BUTTON_R		(1<<8)
#define BUTTON_L		(1<<9)

// #define KEY_DOWN_NOW(key)     0
#define KEY_DOWN_NOW(key)  (~(BUTTONS) & key)
#define BUTTONS 0
//#define BUTTONS *(volatile unsigned int *)0x4000130



typedef struct {
	int row;
	int col;
	int rd;
	int cd;
	int width;
	int height;
} MOVOBJ;

struct RECT_S {
	int row;
	int col;
	int width;
	int height;
	int img;
	int num;
	char deleted;
};

typedef struct RECT_S RECT;


// Prototypes
int detectCollision(MOVOBJ *ball, MOVOBJ *paddle);
int detectCollisionRect(MOVOBJ *ball, RECT *rect);
void waitForVblank();
void gui_set_pixel(int row, int col, unsigned short color);
RECT* createLevel(short level);
void drawLevel();
void freeRectangles();
void allocateRects();

extern RECT* rectangles;
extern int levelSize;
extern int bgcolor;
extern int score;

#endif //MYLIB2_H
