/* shell.c  -  shell */

#include <xinu.h>
#include "shprototypes.h"

/************************************************************************/
/* Table of Xinu shell commands and the function associated with each	*/
/************************************************************************/
const	struct	cmdent	cmdtab[] = {
	{"argecho",	TRUE,	xsh_argecho},
	{"arp",		FALSE,	xsh_arp},
	{"cat",		FALSE,	xsh_cat},
	// RAFA {"clear",	TRUE,	xsh_clear},
	{"clear",	FALSE,	xsh_clear},
	{"date",	FALSE,	xsh_date},
	{"devdump",	FALSE,	xsh_devdump},
	{"echo",	FALSE,	xsh_echo},
	{"exit",	TRUE,	xsh_exit},
	{"help",	FALSE,	xsh_help},
	{"kill",	TRUE,	xsh_kill},
	{"memdump",	FALSE,	xsh_memdump},
	{"memstat",	FALSE,	xsh_memstat},
	{"netinfo",	FALSE,	xsh_netinfo},
	{"ping",	FALSE,	xsh_ping},
	{"ps",		FALSE,	xsh_ps},
	{"sleep",	FALSE,	xsh_sleep},
	{"udp",		FALSE,	xsh_udpdump},
	{"udpecho",	FALSE,	xsh_udpecho},
	{"udpeserver",	FALSE,	xsh_udpeserver},
	{"uptime",	FALSE,	xsh_uptime},
	{"gui_test",	FALSE,	gui_test},
	{"mu_gui_demo",	FALSE,	mu_gui_demo},
	{"mu_clock",	FALSE,	mu_clock},
	{"vt",	FALSE,	vt},
	{"mu_galaga",	FALSE,	mu_galaga},
	{"mu_arkanoid",	FALSE,	mu_arkanoid},
	{"?",		FALSE,	xsh_help},

};

uint32  ncmd = sizeof(cmdtab) / sizeof(struct cmdent);
