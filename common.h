#ifndef __CONFIG_H__
#define __CONFIG_H__

typedef struct
{
	int		threadStarted;
	int		fd_status[2];

	int		hsize;
	int 	wsize;
	int 	wpanel;
	int	 	hdetails;

} commonData;

#endif
