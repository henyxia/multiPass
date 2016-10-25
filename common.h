#ifndef __COMMON_H__
#define __COMMON_H__

#define MAX_MSG_LEN 64

#define INPUT_UNNK		-1
#define INPUT_CMD		0
#define INPUT_TEXTOL	1
#define INPUT_TEXTML	2

typedef struct
{
	int		threadStarted;
	int		fd_input[2];
	int		fd_status[2];

	int		hsize;
	int 	wsize;
	int 	wpanel;
	int	 	hdetails;

} commonData;

int printfd(int, char*, ...);

#endif
