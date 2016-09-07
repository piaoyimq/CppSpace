#include "sem_common.h"

int main(int argc, char *argv[])
{
	int key;
	int semid;
	int nsems;
	int ret;
	int i;
	struct semid_ds	seminfo;
	union semun arg;
	unsigned short *us_arr;
	
	if(2!=argc)
	{
		printf("Usage:\n\t%s <pathname> \n",argv[1]);
		return 2;
	}

	key=ftok(argv[1], 0);
	if(key<0)
	{
		perror("ftok failed");
		return 1;
	}
	semid=semget(key, 0, 0);
	if(semid<0)
	{
		perror("semget failed");
		return 1;
	}

	memset(&seminfo, 0, sizeof(seminfo));
	arg.buf=&seminfo;
	ret=semctl(semid, 0, IPC_STAT, arg);
	if(ret<0)
	{
		perror("semctl failed");
		return 1;
	}

	nsems=arg.buf->sem_nsems;
	ret=nsems*sizeof(unsigned short);
	us_arr=(unsigned short *)malloc(ret);
	if(!us_arr)
	{
		printf("malloc failed\n");
		return 1;
	}
	memset(us_arr, 0, ret);

	arg.array=us_arr;
	ret=semctl(semid, 0, GETALL, arg);
	if(ret<0)
	{
		perror("semctl failed");
		return 1;
	}

	for(i=0; i<nsems; i++)
	{
		printf("semval[%d]=%d\n", i, us_arr[i]);
	}
	return 0;
}

