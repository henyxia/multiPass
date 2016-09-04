#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "common.h"
#include "ui.h"

int main(void)
{
	int			ret;
	pthread_t	t_ui;

	// Creating the common data
	commonData* comm = NULL;
	comm = (commonData*) malloc(sizeof(commonData));
	if(comm == NULL)
	{
		printf("Unalble to allocate the common data\n");
		return 1;
	}
	comm->threadStarted = 0;

	// Init UI
	ret = ui_init(comm);
	if(ret)
	{
		printf("Unable to init the UI properly\n");
		return ret;
	}
	comm->threadStarted++;

	// Init status fd
	ret = pipe(comm->fd_status);
	if(ret<0)
	{
		printf("Unable to create status fd\n");
		return ret;
	}

	// Starting the UI thread
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	ret = pthread_create(&t_ui, &attr, ui_thread, comm);
	if(ret)
	{
		printf("Unable to create the UI thread\n");
		return ret;
	}

	// Sending the status
	FILE* f_status = fdopen(comm->fd_status[1], "w");
	if(f_status == NULL)
	{
		printf("Unable to open FILE to status\n");
		return 1;
	}
	fprintf(f_status, "UI thread successfully initialized");

	// Now let it run
	while(comm->threadStarted > 0)
		sleep(1);

	// Cleaning
	free(comm);

	return 0;
}
