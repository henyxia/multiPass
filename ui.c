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

void* ui_thread(void* comm_raw)
{
	commonData* comm = comm_raw;

	drawUI(comm);

	updateContent(comm);

	comm->threadStarted--;

	return NULL;
}

void drawUI(commonData* comm)
{
	// Clear
	printf("\x1b[?25l\x1b[2J\x1b[0H");

	// Coloring
	//printf("\x1b[37;44m");

	// First
	printf("\u250C");
	for(int i=1; i<(comm->wsize-1); i++)
		printf("\u2500");
	printf("\u2510");
	putchar('\n');

	// Left
	for(int i=1; i<(comm->hsize-1); i++)
		printf("\x1b[%d;1H\u2502", i+1);

	// Right
	for(int i=1; i<(comm->hsize-1); i++)
		printf("\x1b[%d;%dH\u2502", i+1, comm->wsize);

	// Top bar
	printf("\x1b[4;0H\u251C");
	for(int i=1; i<(comm->wsize-1); i++)
		printf("\x1b[4;%dH\u2500", i+1);
	printf("\x1b[4;%dH\u2524", comm->wsize);

	// Bottom bar
	printf("\x1b[%d;0H\u251C", comm->hsize-2);
	for(int i=1; i<(comm->wsize-1); i++)
		printf("\x1b[%d;%dH\u2500", comm->hsize-2, i+1);
	printf("\x1b[%d;%dH\u2524", comm->hsize-2, comm->wsize);

	// Panel
	printf("\x1b[4;%dH\u252C", comm->wpanel+1);
	for(int i=4; i<(comm->hsize-3); i++)
		printf("\x1b[%d;%dH\u2502", i+1, comm->wpanel+1);
	printf("\x1b[%d;%dH\u2534", comm->hsize-2, comm->wpanel+1);

	// Last
	printf("\x1b[%d;0H", comm->hsize);
	printf("\u2514");
	for(int i=1; i<(comm->wsize-1); i++)
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

void updateContent(commonData* comm)
{
	// Vars
	bool			stop = false;
	fd_set			rfds;
	int				max;
	char			buffer[MAX_MSG_LEN];
	int				size;

	FD_ZERO(&rfds);
	FD_SET(comm->fd_status[FD_STDIN], &rfds);
	max = comm->fd_status[FD_STDIN] + 1;

	while(!stop)
	{
		FD_SET(comm->fd_status[FD_STDIN], &rfds);

		select(max, &rfds, NULL, NULL, NULL);

		if(FD_ISSET(comm->fd_status[FD_STDIN], &rfds))
		{
			// Reading
			size = read(comm->fd_status[FD_STDIN], buffer, MAX_MSG_LEN);

			// Printing position (need to be improved)
			printf("\x1b[%d;2H ", comm->hsize-1);
			fflush(stdout);

			// Printing message
			write(STDOUT_FILENO, buffer, size);

			// Removing useless chars
			completeStatusBar(comm->wsize, size);

			// Writing ack
			write(comm->fd_status[FD_ACK_STDOUT], "a", 1);
		}
		else
		{
			// WTF ? We should never be here
			fprintf(stderr, "PANIC... Unexpected behavior in UI thread\n");
		}

		FD_ZERO(&rfds);
	}
}
