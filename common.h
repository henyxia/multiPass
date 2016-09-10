#ifndef __CONFIG_H__
#define __CONFIG_H__

#define MAX_MSG_LEN 64

typedef struct
{
	int		threadStarted;
	int		fd_status[2];

	int		hsize;
	int 	wsize;
	int 	wpanel;
	int	 	hdetails;

} commonData;

int printfd(char*, ...);

#endif
