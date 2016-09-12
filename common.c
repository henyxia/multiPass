#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>

#include "common.h"

int printfd(int fd, char* i_str, ...)
{
	int size, ret;
	char o_str[MAX_MSG_LEN];
	va_list ap;

	size = vsnprintf(o_str, MAX_MSG_LEN, i_str, ap);

	ret = write(fd, o_str, size);

	return ret;
}
