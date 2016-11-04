#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <stdbool.h>
#include <sys/ioctl.h>
#include <sys/select.h>

#include "ui.h"
#include "version.h"

// Private functions
void drawUI(commonData*);
void updateContent(commonData*);

int ui_init(commonData* comm)
{
	if(comm == NULL)
	{
		printf("Unable to initialize the UI configuration\n");
		return 1;
	}

	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

	comm->hsize	= w.ws_row;
	comm->wsize	= w.ws_col;
	comm->wpanel = 25;

	return 0;
}

void* ui_thread(void* uiconf_raw)
{
	commonData* uiconf = uiconf_raw;

	drawUI(uiconf);

	updateContent(uiconf);

	uiconf->threadStarted--;

	return NULL;
}

void drawUI(commonData* uiconf)
{
	// Clear
	printf("\x1b[?25l\x1b[2J\x1b[0H");

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

	fflush(stdout);
}

void completeStatusBar(int max, int current)
{
	for(int i=current; i<(max-3); i++)
	{
		write(STDIN_FILENO, " ", 1);
	}
}

void updateContent(commonData* uiconf)
{
	// Vars
	bool			stop = false;
	fd_set			rfds;
	struct termios	old = { 0 };
	int				max;
	char			buffer[MAX_MSG_LEN];
	int				size;

	// Setting the terminal properly
    if(tcgetattr(0, &old)<0)
        perror("tcsetattr()");
    old.c_lflag&=~ICANON;
   	old.c_lflag &= ~ECHO;
    old.c_cc[VMIN]=1;
    old.c_cc[VTIME]=0;
    if(tcsetattr(0, TCSANOW, &old)<0)
        perror("tcsetattr ICANON");

	FD_ZERO(&rfds);
	FD_SET(STDIN_FILENO, &rfds);
	FD_SET(uiconf->fd_status[FD_STDIN], &rfds);
	max = uiconf->fd_status[FD_STDIN] + 1;

	while(!stop)
	{
		FD_SET(STDIN_FILENO, &rfds);
		FD_SET(uiconf->fd_status[FD_STDIN], &rfds);

		select(max, &rfds, NULL, NULL, NULL);

		if(FD_ISSET(uiconf->fd_status[FD_STDIN], &rfds))
		{
			// Reading
			size = read(uiconf->fd_status[FD_STDIN], buffer, MAX_MSG_LEN);

			// Printing position (need to be improved)
			printf("\x1b[%d;2H ", uiconf->hsize-1);
			fflush(stdout);

			// Printing message
			write(STDOUT_FILENO, buffer, size);

			// Removing useless chars
			completeStatusBar(uiconf->wsize, size);

			// Writing ack
			write(uiconf->fd_status[FD_ACK_STDOUT], "a", 1);
		}
		else if(FD_ISSET(STDIN_FILENO, &rfds))
		{
			size = read(STDIN_FILENO, buffer, MAX_MSG_LEN);
			printf("\x1b[%d;2H Stdin ? %c", uiconf->hsize-1, buffer[0]);
			fflush(stdout);
			completeStatusBar(uiconf->wsize, size+8);
		}

		FD_ZERO(&rfds);
	}
}
