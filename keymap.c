#include "keymap.h"

void keymap_default_config(keymap* k)
{
	k[REFRESH].cmd 			= 'l';
	k[SIDEBAR_ON].cmd		= 'o';
	k[SIDEBAR_OFF].cmd		= 'p';
	k[SIDEBAR_SWITCH].cmd	= 'i';
	k[QUIT].cmd				= 'q';

}
