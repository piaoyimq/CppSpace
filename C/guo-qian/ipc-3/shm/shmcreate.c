#include "shm_common.h"

int main(int argc, char *argv[])
{
	int key;
	int shmid;
	int length;
	int ret;
	int *ptr;

	if(3!=argc)
	{
		printf("Usage:\n\t%s <pathname> <size>\n",argv[0]);
		return 2;
	}

	key=ftok(argv[1], 0);
	if(key<0)
	{
		perror("ftok failed");
		return 1;
	}
	length=atoi(argv[2]);
	if(length<0)
	{
		printf("Usage:\n\t%s <pathname> <size>\n",argv[0]);
		return 2;
	}
	shmid=shmget(key, length, 0664|IPC_CREAT);
	if(shmid<0)
	{
		perror("shmget failed");
		return 1;
	}

	ptr=(int *)shmat(shmid, NULL, 0);
	if(-1==(int)ptr)
	{
		perror("shmat failed");
		return 1;
	}

	*ptr=0;
	return 0;
}

