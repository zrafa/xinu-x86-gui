/* xsh_ls.c - xsh_ls */

#include <xinu.h>
#include <stdio.h>
#include <string.h>


shellcmd xsh_ls(int nargs, char *args[])
{
    /* if (nargs > 1) {
        printf("%s\n", "Too many arguments.");
        return 0;
    } */
    //struct rfdata curr_rfdata = Rf_data;
    int i;
    for (i = 0; rfltab[i].rfname[0] != '\0'; i++) {
        printf("%s\n", rfltab[i].rfname);
    }

    if (i == 0) {
        printf("%s\n", "There are no files in the current directory.");
    }

}