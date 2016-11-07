#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <stdbool.h>
#include <sys/ioctl.h>
#include <sys/select.h>

#include "input.h"

// Private function
void inputProcess(commonData*);

// Public function
int input_init(void)
{
	struct termios	old = { 0 };
    int ret;

 	// Setting the terminal properly
    if(tcgetattr(0, &old)<0)
        perror("tcsetattr()");
    old.c_lflag&=~ICANON;
   	old.c_lflag &= ~ECHO;
    old.c_cc[VMIN]=1;
    old.c_cc[VTIME]=0;

    ret = tcsetattr(0, TCSANOW, &old);
    if(ret<0)
        perror("tcsetattr ICANON");

    return ret;
}

void* input_thread(void* comm_raw)
{
	commonData* comm = comm_raw;

	inputProcess(comm);

	comm->threadStarted--;

	return NULL;
}

void inputProcess(commonData* comm)
{
	// Vars
	bool			stop = false;
	fd_set			rfds;
	int				max;
	char			buffer[MAX_MSG_LEN];
	int				size;

	FD_ZERO(&rfds);
	FD_SET(STDIN_FILENO, &rfds);
	max = 1;

	while(!stop)
	{
		FD_SET(STDIN_FILENO, &rfds);

		select(max, &rfds, NULL, NULL, NULL);

		if(FD_ISSET(STDIN_FILENO, &rfds))
		{
			size = read(STDIN_FILENO, buffer, MAX_MSG_LEN);
			//printfd(comm->fd_status, "Input len");
			buffer[size] = 0;
			printfd(comm->fd_status, "> lol %c", buffer[0]);
			//printf("\x1b[%d;2H Stdin ? %c", comm->hsize-1, buffer[0]);
			//fflush(stdout);
			//completeStatusBar(comm->wsize, size+8);
		}
		else
		{
			// WTF ? We should never be here
			fprintf(stderr, "PANIC... Unexpected behavior in INPUT thread\n");
		}

		FD_ZERO(&rfds);
	}
}
