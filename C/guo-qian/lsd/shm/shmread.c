#include "shm_common.h"

int main(int argc, char *argv[])
{
	int key;
	int shmid;
	int *ptr;
	int ret;
	int i;

	if(2!=argc)
	{
		printf("Usage:\n\t%s <pathname>\n",argv[0]);
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
	ptr=(int*)shmat(shmid, NULL, 0);
	if(-1==(int)ptr)
	{
		perror("shmat failed");
		return 1;
	}
	
	printf("%d\n", *ptr);
	
	return 0;
}

