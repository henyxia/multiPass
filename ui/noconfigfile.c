#include <stdio.h>
#include <string.h>

#include "noconfigfile.h"

ui_input_command* noconfigfile_init()
{
	ui_input_command* ret = create_ui_input_command(1);

	if(ret == NULL)
	{
		fprintf(stderr, "Unable to init the ui input command\n");
		return ret;
	}

	ret[0].command = 'q';
	strcpy(ret[0].description, "quit");
	ret[0].length = 4;

	return ret;
}

int noconfigfile_render(commonData* comm)
{
	printfd(comm->fd_status, "Loading no config file found prompt");
	return 0;
}
