/* kbd.h : ps/2 keyboard struct for kbd control driver */

#define inportb(p)      inb(p)
#define outportb(p,v)   outb(p,v)

#define KEYBOARD_DATAREG 0x60  // :Data Register(Read\Write)
#define KEYBOARD_CMDREG 0x64   // :Command Register(Read\Write)


//extern unsigned char kblayout [128];  // { ... } Fill your layout yourself

struct  kbdcblk {                       /* Kbd line control block       */
        unsigned char   key;            /* key data */
        sid32   kbdsem;                 /* Input semaphore              */
};

extern  struct  kbdcblk kbdc;
