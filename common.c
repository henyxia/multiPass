#include <stdio.h>
#include <stdarg.h>

#include "common.h"

int printfd(char* i_str, ...)
{
	int ret;
	char o_str[MAX_MSG_LEN];
	va_list ap;

	ret = vsnprintf(o_str, MAX_MSG_LEN, i_str, ap);

	return ret;
}
