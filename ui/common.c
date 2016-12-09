#include <stdio.h>
#include <stdlib.h>

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

void createMultipleOption(commonData* comm, char* title, ...)
{
	//va_list	ap;
	//int 	height 	= 8;
	//int 	width	= 4;

	int 	maxLength = 0;

	char	line1[MAX_MSG_LEN];
	char	line2[MAX_MSG_LEN];
	char	line3[MAX_MSG_LEN];

	maxLength = strlen(title);

	sprintf(line1, "+---+");
	sprintf(line2, " %s ", title);
	sprintf(line3, "+---+");

	printfd(comm->fd_content, title);
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
