#include "sem_common.h"

int main(int argc, char *argv[])
{
	int key;
	int oflag=0640|IPC_CREAT;
	int semid;
	int nsems;
	
	if(3!=argc)
	{
		printf("Usage:\n\t%s <pathname> <nsems>\n",argv[1]);
		return 2;
	}

	key=ftok(argv[1], 0);
	if(key<0)
	{
		perror("ftok failed");
		return 1;
	}
	nsems=atoi(argv[2]);
	semid=semget(key, nsems, oflag);
	if(semid<0)
	{
		perror("semget failed");
		return 1;
	}
	return 0;
}

