#include "sem_common.h"

#define OPERATION_OFFSET		2

int main(int argc, char *argv[])
{
	int key;
	int semid;
	int i;
	int ops_num;
	int ret;
	struct sembuf	*buf;
	
	if(3>argc)
	{
		printf("Usage:\n\t%s <pathname> [operations...] \n",argv[1]);
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
	ops_num=argc-OPERATION_OFFSET;
	
	ret=(ops_num)*sizeof(struct sembuf);

	buf=(struct sembuf*)malloc(ret);
	if(!buf)
	{
		printf("malloc failed\n");
		return 1;
	}

	memset(buf, 0, ret);
	
	for(i=0; i<ops_num; i++)
	{
		buf[i].sem_num=i;
		buf[i].sem_op=atoi(argv[i+OPERATION_OFFSET]);
	}

	ret=semop(semid, buf, ops_num);
	if(ret<0)
	{
		perror("semop failed\n");
		return 1;
	}
		
	return 0;
}

