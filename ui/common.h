#ifndef __UI_COMMON_H__
#define __UI_COMMON_H__

#define MAX_COMMAND_DESCRIPTION	32

#define UI_NOTHING		0
#define	UI_NOCONFIGFILE	1

#include "../common.h"

typedef struct
{
	char			command;
	char			description[MAX_COMMAND_DESCRIPTION];
	unsigned int 	length;
} ui_input_command;

void				init_ui_common(commonData*);
ui_input_command* 	create_ui_input_command(int);
void				delete_ui_input_command(ui_input_command*);

void				createMultipleOption(commonData*, char*, ...);

#endif
