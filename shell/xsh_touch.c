#include <xinu.h>
#include <stdio.h>
#include <string.h>

shellcmd xsh_touch(int nargs, char *args[])
{
    int * fd;

	fd = open(RFILESYS, args[1], "rw");
	int retval1 = write(fd, "this is a buffer\n", 17); 
    close(fd); 
}

