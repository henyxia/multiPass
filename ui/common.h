#ifndef __UI_COMMON_H__
#define __UI_COMMON_H__

#define MAX_COMMAND_DESCRIPTION	32

#define UI_NOTHING	0

typedef struct
{
	char			command;
	char			description[MAX_COMMAND_DESCRIPTION];
	unsigned int 	length;
} ui_input_command;

ui_input_command* 	create_ui_input_command(int);
void				delete_ui_input_command(ui_input_command*);

#endif
