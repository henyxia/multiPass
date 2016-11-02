#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/sem.h>

#include "common.h"

#define KEY	0x1100

// Private functions
int 	lockIO(int);
int 	unlockIO(int);
int 	getSemFromFd(int);
int 	get_sem(int);
void 	show_sem(int);
void 	create_sem(int);
void 	init_sem(int);
void 	initThreads(int);
int 	PV(int, int);
int 	P(int);
int 	V(int);

// Global
int	mySem;
int matchingFd[FD_MAX];

// printfd
int printfd(int fd, char* i_str, ...)
{
	va_list	ap;
	char 	o_str[MAX_MSG_LEN];
	int 	semId;
	int 	size;
	int 	ret;

	// Locking the sem
	semId = getSemFromFd(fd);
	if(semId<0)
	{
		printf("Unable to get the semaphore from its fd\n");
		return semId;
	}
	lockIO(semId);

	// Rendering to str
	size = vsnprintf(o_str, MAX_MSG_LEN, i_str, ap);

	// Writing the output
	ret = write(fd, o_str, size);

	// Unlocking the sem
	unlockIO(semId);

	return ret;
}

int getSemFromFd(int fd)
{
	for(int i=0; i<FD_MAX; i++)
		if(matchingFd[i] == fd)
			return i;

	return -1;
}

// lockIO
int lockIO(int io)
{ 
	if(io != INPUT && io != OUTPUT_CMD && io != OUTPUT_OL && io != OUTPUT_ML)
	{
		// Unrecognized io
		return -1;
	}

	return P(io);
}

// unlockIO
int unlockIO(int io)
{
	if(io != INPUT && io != OUTPUT_CMD && io != OUTPUT_OL && io != OUTPUT_ML)
	{
		// Unrecognized io
		return -1;
	}

	return V(io);
 }

// Private functions
int get_sem(int i)
{
	unsigned short int semarr[30];
	union semun
	{
		int val;
		struct semid_ds *buf;
		unsigned short int *array;
	}arg;

	arg.array = semarr;
	semctl(mySem, i, GETALL, arg);
	return semarr[i];
}

void show_sem(int i)
{
	int val;
	val = get_sem(i);
	printf("semaphore[%d]=%d\n", i, val);
}

void create_sem(int N)
{
#ifdef DEBUG
	printf("Creating %d semaphores\n", N);
#endif
	mySem = semget(KEY, N, 0666 | IPC_CREAT);
	if(mySem < 0)
		printf("Unable to create the semaphore\n");
}

void init_sem(int N)
{
	int j;
	int retval;
	union semun
	{
		int val;
		struct semid_ds *buf;
		unsigned short int *array;
	}arg;
	arg.val = 1;
	for (j=0; j<N; j++)
	{
		retval = semctl(mySem, j, SETVAL, arg);
		if(retval < 0)
			printf("Unable to initialize the semaphore\n");
	}
}

void initSemaphores(int sem)
{
	create_sem(sem);
	init_sem(sem);
}


int PV(int i, int act)
{
	struct sembuf op;

	op.sem_num = i;
	op.sem_op = act; 	// 1=V, -1=P
	op.sem_flg = 0; 	// Will wait
	return semop(mySem, &op, 1);
}

int P(int i)
{
	return PV(i, -1);
}

int V(int i)
{
	return PV(i, 1);
}
