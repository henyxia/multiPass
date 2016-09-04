#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "ui.h"

char lastMessage[255];

void setLastMessage(const char* mes)
{
	strcpy(lastMessage, mes);
}

int main(void)
{
	int ret;

	// Creating the common data
	commonData* comm = NULL;
	comm = (commonData*) malloc(sizeof(commonData));
	if(comm == NULL)
	{
		printf("Unalble to allocate the common data\n");
		return 1;
	}

	// Init UI
	ret = ui_init(comm);
	if(ret)
	{
		printf("Unable to init the UI properly\n");
		return ret;
	}

	setLastMessage("Starting server\n");

	// Starting the UI thread
	drawUI();

	// Cleaning
	free(comm);

	return 0;
}
