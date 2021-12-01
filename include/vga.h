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

extern	struct	vga_t	vga;
