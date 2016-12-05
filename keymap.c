#include "keymap.h"

void keymap_default_config(keymap* k)
{
	k[REFRESH].map 			= 'l';
	k[SIDEBAR_ON].map		= 'o';
	k[SIDEBAR_OFF].map		= 'p';
	k[SIDEBAR_SWITCH].map	= 'i';
	k[QUIT].map				= 'q';

}
