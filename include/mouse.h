/* mouse.h - definitions for the mouse device */

/* mouse device */

typedef struct	mouse	{
	int buttons;
	int x;
	int y;
	} mouse_t;

struct  mousecblk {             /* mouse control block  */
        mouse_t mouse;          /* mouse data           */
        sid32   mousesem;       /* mouse semaphore      */
};

extern	struct	mouse_t	mouse;
extern  struct  mousecblk mousec;
