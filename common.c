#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/sem.h>
#include <errno.h>

#include "common.h"

#define KEY	0x1100

#ifdef DEBUG
FILE* stdERR;
int init_errorlog(void);
int print_errorlog(char*, ...);
#else
int print_errorlog(char* i, ...)
{
	char tmp;
	tmp = i[0];
	tmp = 0;
	return tmp;
}
#endif

// Private functions
void 	initSemaphores(int);
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
int matchingFd[FDS_MAX];

int common_init()
{
#ifdef DEBUG
	int ret;

	ret = init_errorlog();
	if(ret<0)
	{
		fprintf(stderr, "Unable to start error log\n");
		fflush(stdout);
		return ret;
	}
#endif

	initSemaphores(FDS_MAX);

	for(int i=0; i<FDS_MAX; i++)
		matchingFd[i]=-1;

	return 0;
}

// printfd
int printfd(int* fds, char* i_str, ...)
{
	va_list	ap;
	char	dc[MAX_ACK_LEN];
	char 	o_str[MAX_MSG_LEN];
	int 	semId;
	int 	size;
	int 	ret;

	print_errorlog("------------------------------------\n");
	print_errorlog("Going to print though fd(%d) message\n", fds[FD_STDOUT]);

	// Getting the va
	va_start(ap, i_str);

	// Locking the sem
	semId = getSemFromFd(fds[FD_STDOUT]);
	print_errorlog("Locking semaphore (%d)\n", semId);
	if(semId<0)
	{
		print_errorlog("Unable to get the semaphore\n");
		return semId;
	}
	lockIO(semId);

	// Rendering to str
	size = vsnprintf(o_str, MAX_MSG_LEN, i_str, ap);
	print_errorlog("Message is (size %d)\n-> %s\n", size, o_str);

	// Writing the output
	ret = write(fds[FD_STDOUT], o_str, size);

	// Waiting for the ack
	read(fds[FD_ACK_STDIN], dc, MAX_ACK_LEN);

	// Unlocking the sem
	print_errorlog("Unlocking semaphore (%d)\n", semId);
	unlockIO(semId);

	return ret;
}

int initFds(int io, int* fds)
{
	int ret;
	int pipeTmp[2];

	if(io <= FDS_MIN || io >= FDS_MAX)
		return -1;

	// Init the two first fd
	ret = pipe(fds);
	if(ret<0)
	{
		printf("\nUnable to create fd\n");
		return ret;
	}

	// Then the next two
	ret = pipe(pipeTmp);
	if(ret<0)
	{
		printf("Unable to create ack fd\n");
		return ret;
	}

	// Storing
	fds[FD_ACK_STDIN] = pipeTmp[FD_STDIN];
	fds[FD_ACK_STDOUT] = pipeTmp[FD_STDOUT];

	// Keeping for the sem
	matchingFd[io]=fds[FD_STDOUT];

	return 0;
}

int getSemFromFd(int fd)
{
	for(int i=0; i<FDS_MAX; i++)
		if(matchingFd[i] == fd)
			return i;

	return -1;
}

#ifdef DEBUG
// init_errorlog
int init_errorlog(void)
{
	stdERR = fopen("error.log", "a+");
	if(stdERR==NULL)
	{
		fprintf(stderr, "Unable to open stderr file");
		return -1;
	}

	print_errorlog("\n=== STARTING ===\n\n");

	return 0;
}

int print_errorlog(char* str, ...)
{
	va_list ap;
	int		ret;

	// Getting the va
	va_start(ap, str);

	ret = vfprintf(stdERR, str, ap);
	fflush(stdERR);

	return ret;
}
#endif
// lockIO
int lockIO(int io)
{ 
	if(io <= FDS_MIN || io >= FDS_MAX)
		// Unrecognized io
		return -1;

	return P(io);
}

// unlockIO
int unlockIO(int io)
{
	if(io <= FDS_MIN || io >= FDS_MAX)
		// Unrecognized io
		return -1;

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
	print_errorlog("Creating %d semaphores\n", N);
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
