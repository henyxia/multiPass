#ifndef __KEYMAP_H__
#define __KEYMAP_H__

#include <stdbool.h>

#define MIN_KEYMAP		-1
#define REFRESH			0
#define	SIDEBAR_ON		1
#define	SIDEBAR_OFF		2
#define SIDEBAR_SWITCH	3
#define QUIT			4
#define MAX_KEYMAP		5

#define MAX_KEYMAP_DESCRIPTION	16

typedef struct
{
	char	cmd;
	char	desc[MAX_KEYMAP_DESCRIPTION];
	bool	isCtrl;
	bool	isAlt;
	bool	isShift;
} keymap;

void keymap_default_config(char*);

#endif
