/* mouse.h - definitions for the mouse device */

/* mouse device */

#define MOUSE_DATA_PORT 0x60
#define MOUSE_CTRL_STATUS_PORT 0x64

#define inportb(p)      inb(p)
#define outportb(p,v)   outb(p,v)


struct	mouse_t	{
	int buttons;
	int x;
	int y;
	};

extern	struct	mouse_t	mouse;


/* in miceutils.c */

extern void mouse_wait(unsigned char a_type); //unsigned char
extern void mouse_write(unsigned char a_write); //unsigned char
extern unsigned char mouse_read();
