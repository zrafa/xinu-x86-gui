/* delay.h - DELAY */

/*
 * Delay units are in microseconds.
 */
#define	DELAY(n)                                	\
{                                               	\
	register int i = 0;				\
							\
	while(i < (n * 100)) i++;			\
}							\

/*
 * Delay units are in milli-seconds.
 */
#define	MDELAY(n)					\
{							\
	register long i;				\
							\
	for (i=n;i>0;i--) {				\
		DELAY(1000);				\
	}						\
}
