#ifndef __COMMON_H__
#define __COMMON_H__

#define MAX_MSG_LEN 64
#define MAX_ACK_LEN 2

#define FDS_MIN			0
#define FDS_STATUS		1
#define FDS_UICONTROL	2
#define	FDS_MAX			3

#define FD_STDIN		0
#define	FD_STDOUT		1
#define FD_ACK_STDIN	2
#define FD_ACK_STDOUT	3

#define CLEAR_SCREEN		'0'
#define SIDEBAR_TOGGLE_OFF	'1'
#define SIDEBAR_TOGGLE_ON	'2'


typedef struct commonData
{
	int		threadStarted;
	int		fd_input[4];
	int		fd_status[4];
	int		fd_uicontrol[4];

	int		sidebar;

	int		hsize;
	int 	wsize;
	int 	wpanel;
	int	 	hdetails;

	void(* switchToUI) (int);

} commonData;

int common_init();
int printfd(int*, char*, ...);
int initFds(int, int *);

#endif
