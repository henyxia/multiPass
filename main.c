#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "common.h"
#include "config.h"
#include "input.h"
#include "ui.h"

int main(void)
{
	int			ret;
	pthread_t	t_config;
	pthread_t	t_input;
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

	// Init Common
	ret = common_init();
	if(ret<0)
	{
		printf("Unable to initialize the common data\n");
		return ret;
	}

	// Init UI
	ret = ui_init(comm);
	if(ret)
	{
		printf("Unable to init the UI properly\n");
		return ret;
	}

	// Init status fd
	ret = initFds(comm->fd_status);
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
	comm->threadStarted++;

	// Sending the status
	printfd(comm->fd_status, "UI thread successfully initialized");



	// Init UI
	ret = input_init();
	if(ret)
	{
		printf("Unable to init the input properly\n");
		return ret;
	}

	// Starting the input thread
	pthread_attr_t attr_input;
	pthread_attr_init(&attr_input);
	pthread_attr_setdetachstate(&attr_input, PTHREAD_CREATE_DETACHED);
	ret = pthread_create(&t_input, &attr_input, input_thread, comm);
	if(ret)
	{
		printf("Unable to create the UI thread\n");
		return ret;
	}
	comm->threadStarted++;

	// Sending the status
	printfd(comm->fd_status, "Input thread successfully initialized");

	// Init Common UI
	init_ui_common(comm);

	// Parsing the configuration
	pthread_attr_t attr2;
	pthread_attr_init(&attr2);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	ret = pthread_create(&t_config, &attr2, config_thread, comm);
	if(ret)
	{
		printf("Unable to create the conf thread\n");
		return ret;
	}
	comm->threadStarted++;

	// Now let it run
	while(comm->threadStarted > 0)
		sleep(1);

	// Cleaning
	free(comm);

	return 0;
}
