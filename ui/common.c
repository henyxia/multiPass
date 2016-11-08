#include <stdlib.h>

#include "common.h"

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
