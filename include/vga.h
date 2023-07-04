/* vga.h - definitions for the vga screen */

/* vga screen */

struct	vga_t	{
	uint8 *addr;
	uint32 width;
	uint32 height;
	uint32 pitch;
	uint8 bpp;
	uint32 pos;
	};

extern	struct	vga_t	*vga;


/* vga control functions */

#define VGA_GET_PITCH 0 
#define VGA_GET_BPP 1
#define VGA_GET_WIDTH 2
#define VGA_GET_HEIGHT 3
#define VGA_GET_POS 4
