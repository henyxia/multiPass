#include <stdio.h>
#include <unistd.h> // For the timer, need to be deleted soon
#include <sys/stat.h>
#include <sys/types.h>

#include "config.h"

// Private functions
void readConfig(commonData*);

int config_init(commonData* comm)
{
	if(comm == NULL)
	{
		printf("Unable to initialize the UI configuration\n");
		return 1;
	}

	comm->wpanel = 25;

	return 0;
}

void* config_thread(void* commonData_raw)
{
	commonData* comm = commonData_raw;

	readConfig(comm);

	return NULL;
}

void readConfig(commonData* comm)
{
	FILE* config_file = NULL;

	config_file = fopen("~/.multiPass.rc", "r");
	if(config_file == NULL)
	{
		printfd(comm->fd_status, "Unable to open configuration file");
		comm->switchToUI(UI_NOCONFIGFILE);
		return;
	}

	// Checking the mask (must be 0600)

	// Reading configuration
}

void initConfiguration(commonData* comm)
{
	FILE* config_file = fopen("~/.multiPass.rc", "w+");
	if(config_file == NULL)
	{
		printfd(comm->fd_status, "Unable to open configuration file");
	}

	fprintf(config_file, "ui\n");
	fprintf(config_file, "\tpanel_width=25\n");
	// Maybe a strong authentication here
	fflush(config_file);

	fclose(config_file);
}

