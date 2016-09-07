#include "sem_common.h"

int main(int argc, char *argv[])
{
	int key;
	int semid;
	int ret;
	
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

	ret=semctl(semid, 0, IPC_RMID);
	if(ret<0)
	{
		perror("semctl failed");
		return 1;
	}
	return 0;
}

