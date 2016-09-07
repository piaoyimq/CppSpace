#ifndef _SEM_COMMON_H_
#define _SEM_COMMON_H_

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

union semun{
	int 	val;
	struct semid_ds	*buf;
	unsigned short	*array;
};

#endif

