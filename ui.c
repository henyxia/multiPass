#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "ui.h"
#include "version.h"

typedef struct
{
	int		hsize;
	int 	wsize;
	int 	wpanel;
	int	 	hdetails;
} ui_config;

typedef struct
{
	char		name[64];
} command;

commonData* uiconf = NULL;

int ui_init(commonData* comm)
{
	uiconf = comm;

	if(uiconf == NULL)
	{
		printf("Unable to initialize the UI configuration\n");
		return 1;
	}

	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

	uiconf->hsize	= w.ws_row;
	uiconf->wsize	= w.ws_col;
	uiconf->wpanel	= 25;

	return 0;
}

void drawUI()
{
	// Clear
	printf("\x1b[2J\x1b[0H");

	// Coloring
	printf("\x1b[37;44m");

	// First
	printf("\u250C");
	for(int i=1; i<(uiconf->wsize-1); i++)
		printf("\u2500");
	printf("\u2510");
	putchar('\n');

	// Left
	for(int i=1; i<(uiconf->hsize-1); i++)
		printf("\x1b[%d;1H\u2502", i+1);

	// Right
	for(int i=1; i<(uiconf->hsize-1); i++)
		printf("\x1b[%d;%dH\u2502", i+1, uiconf->wsize);

	// Top bar
	printf("\x1b[4;0H\u251C");
	for(int i=1; i<(uiconf->wsize-1); i++)
		printf("\x1b[4;%dH\u2500", i+1);
	printf("\x1b[4;%dH\u2524", uiconf->wsize);

	// Bottom bar
	printf("\x1b[%d;0H\u251C", uiconf->hsize-2);
	for(int i=1; i<(uiconf->wsize-1); i++)
		printf("\x1b[%d;%dH\u2500", uiconf->hsize-2, i+1);
	printf("\x1b[%d;%dH\u2524", uiconf->hsize-2, uiconf->wsize);

	// Panel
	printf("\x1b[4;%dH\u252C", uiconf->wpanel+1);
	for(int i=4; i<(uiconf->hsize-3); i++)
		printf("\x1b[%d;%dH\u2502", i+1, uiconf->wpanel+1);
	printf("\x1b[%d;%dH\u2534", uiconf->hsize-2, uiconf->wpanel+1);

	// Last
	printf("\x1b[%d;0H", uiconf->hsize);
	printf("\u2514");
	for(int i=1; i<(uiconf->wsize-1); i++)
		printf("\u2500");
	printf("\u2518");

	// Title
	printf("\x1b[1;3H\u2524 mutliPass %d.%d \u251C", VERSION_MAJOR,
			VERSION_MINOR);

	// Temp final display
	printf("\x1b[%d;0H", uiconf->hsize);
	putchar('\n');
}

