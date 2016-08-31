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

	uiconf->hsize = 36;
	uiconf->wsize = 128;
	uiconf->wpanel	= 15;

	return 0;
}

void drawUI()
{
	//printf("%s\n", HOME);

	// First
	for(int i=0; i<uiconf->wsize; i++)
		putchar('-');
	putchar('\n');

	// Body
	for(int j=2; j<uiconf->hsize; j++)
	{
	putchar('|');
	for(int i=1; i<(uiconf->wsize-1); i++)
	{
		putchar(i == uiconf->wpanel ? '|' : ' ');
	}
	putchar('|');
	putchar('\n');
	}

	// Last
	for(int i=0; i<uiconf->wsize; i++)
		putchar('-');
	putchar('\n');
}

