#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "common.h"
#include "nothing.h"
#include "noconfigfile.h"

commonData* _internal_ui_comm;

void switchToUI(int);

void init_ui_common(commonData* comm)
{
	_internal_ui_comm = comm;
	comm->switchToUI = switchToUI;
};

ui_input_command* create_ui_input_command(int size)
{
	ui_input_command* ret = NULL;

	ret = (ui_input_command*) malloc(sizeof(ui_input_command)*size);

	return ret;
}

void delete_ui_input_command(ui_input_command* old)
{
	free(old);
}

char* _generateBar(int len)
{
	char* out = NULL;

	out = (char*) malloc(sizeof(char)*(len+1));
	if(out == NULL)
	{
		return NULL;
	}

	for(int i=0;i<len;i++)
		out[i] = '-';
	out[len] = 0;

	return out;
}

void createMultipleOption(commonData* comm, char* title, ...)
{
	va_list	ap;
	int 	height 	= 8;
	int 	width	= 3;

	int options = 0;

	int 	maxLength = 0;

	char	conf[MAX_MSG_LEN];
	char	line[MAX_MSG_LEN];

	maxLength = strlen(title);

	// Conf
	sprintf(conf, "%d,%d", height, width);
	printfd(comm->fd_content, conf);

	// Line 1
	sprintf(line, "+-%s-+", _generateBar(maxLength));
	printfd(comm->fd_content, line);

	// Line 2
	sprintf(line, "| %s |", title);
	printfd(comm->fd_content, line);

	// Line 3
	sprintf(line, "+-%s-+", _generateBar(maxLength));
	printfd(comm->fd_content, line);

	// Line 4
	va_start(ap, title);
	while(*title)
	{
		options++;
	}
	sprintf(line, "| Found %d options |", options);
	printfd(comm->fd_content, line);

	// Line 5
	sprintf(line, "+-%s-+", _generateBar(maxLength));
	printfd(comm->fd_content, line);
}

// Private function
void switchToUI(int ui)
{
	switch(ui)
	{
		case UI_NOTHING:
		break;
		case UI_NOCONFIGFILE:
			noconfigfile_init();
			noconfigfile_render(_internal_ui_comm);
		break;
	}
	ui++;
	return;
}
