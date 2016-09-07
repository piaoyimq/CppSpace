#include "shm_common.h"

int main(int argc, char *argv[])
{
	int key;
	int shmid;
	int value;
	char *ptr;
	int ret;
	int i;
	struct shmid_ds info;

	if(3!=argc)
	{
		printf("Usage:\n\t%s <pathname> <value>\n",argv[0]);
		return 2;
	}

	key=ftok(argv[1], 0);
	if(key<0)
	{
		perror("ftok failed");
		return 1;
	}
	shmid=shmget(key, 0, 0);
	if(shmid<0)
	{
		perror("shmget failed");
		return 1;
	}
	ptr=shmat(shmid, NULL, 0);
	if(-1==(int)ptr)
	{
		perror("shmat failed");
		return 1;
	}

	ret=shmctl(shmid, IPC_STAT, &info);
	if(ret<0)
	{
		perror("shctl failed");
		return 1;
	}

	
	value=atoi(argv[2]);
	for(i=0; i<info.shm_segsz; i++)
	{
		ptr[i]=value;
	}
	
	return 0;
}

