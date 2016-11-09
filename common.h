#ifndef __COMMON_H__
#define __COMMON_H__

#define MAX_MSG_LEN 64
#define MAX_ACK_LEN 2

#define	INPUT			0
#define OUTPUT_CMD		1
#define OUTPUT_OL		2
#define OUTPUT_ML		3
#define	FD_MAX			4

#define FD_STDIN		0
#define	FD_STDOUT		1
#define FD_ACK_STDIN	2
#define FD_ACK_STDOUT	3

typedef struct
{
	int		threadStarted;
	int		fd_input[4];
	int		fd_status[4];

	int		hsize;
	int 	wsize;
	int 	wpanel;
	int	 	hdetails;

	void(* switchToUI) (int);

} commonData;

int common_init();
int printfd(int*, char*, ...);

#endif
