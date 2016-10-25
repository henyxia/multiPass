#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>

#include "common.h"

// Private functions
int get_sem(int);
void show_sem(int);
void create_sem(int);
void init_sem(int);
void initThreads(int);
void PV(int, int);
void P(int);
void V(int);

// printfd
int printfd(int fd, char* i_str, ...)
{
	int size, ret;
	char o_str[MAX_MSG_LEN];
	va_list ap;

	// Locking the sem


	// Rendering to str
	size = vsnprintf(o_str, MAX_MSG_LEN, i_str, ap);

	// Writing the output
	ret = write(fd, o_str, size);

	// Unlocking the sem


	return ret;
}

// Private functions
int get_sem(int i)
{
	ushort semarr[30];
	union semun
	{
		int val;
		struct semid_ds *buf;
		ushort *array;
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
		ushort *array;
	}arg;
	arg.val = 1;
	for (j=0; j<N; j++)
	{
		retval = semctl(mySem, j, SETVAL, arg);
		if(retval < 0)
			printf("Unable to initialize the semaphore\n");
	}
}

void initThreads(int sem)
{
	create_sem(sem);
	init_sem(sem);
}


void PV(int i, int act)
{
	struct sembuf op;
	int retval;

	op.sem_num = i;
	op.sem_op = act; //1=V, -1=P
	op.sem_flg = 0; //will wait
	retval = semop(mySem, &op, 1);
	if(retval != 0)
		printf("Error semop will do %d\n", act);
}

void P(int i)
{
	PV(i, -1);
}

void V(int i)
{
	PV(i, 1);
}
