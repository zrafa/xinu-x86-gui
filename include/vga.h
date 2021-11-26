/* vga.h - definitions for the vga screen */

/* vga screen */

#define	VGA_WIDHT 1024
#define	VGA_HEIGHT 768

struct	vga_t	{
	unsigned char *addr;
	unsigned int pos;
	};

extern	struct	vga_t	vga;
