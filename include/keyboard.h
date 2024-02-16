/* kbd.h : ps/2 keyboard struct for kbd control driver */

struct  kbdcblk {                       /* Kbd line control block       */
        unsigned char   key;            /* key data */
        sid32   kbdsem;                 /* Input semaphore              */
};

extern  struct  kbdcblk kbdc;
