#ifndef __COMMON_H__
#define __COMMON_H__

#define MAX_MSG_LEN 64

#define	INPUT			0
#define OUTPUT_CMD		1
#define OUTPUT_OL		2
#define OUTPUT_ML		3
#define	FD_MAX			4

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
