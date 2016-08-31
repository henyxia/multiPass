#include <stdio.h>
#include <string.h>

#include "ui.h"

char lastMessage[255];

void setLastMessage(const char* mes)
{
	strcpy(lastMessage, mes);
}

int main(void)
{
	ui_init();

	setLastMessage("Starting server\n");

	drawUI();

	return 0;
}
