#include <stdio.h>
#include <stdlib.h>

#include "ui.h"

#define HOME 	"aaa"
#define	CLEAR	"aaa"

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

ui_config* uiconf = NULL;

int ui_init()
{
	uiconf = (ui_config*) malloc( sizeof( ui_config ) );

	if(uiconf == NULL)
	{
		printf("Unable to initialize the UI configuration\n");
		return 1;
	}

	uiconf->hsize	= 36;
	uiconf->wsize	= 128;
	uiconf->wpanel	= 25;

	return 0;
}

void drawUI()
{
	// Clear
	printf("\x1b[2J\x1b[0H");

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

	putchar('\n');
}

