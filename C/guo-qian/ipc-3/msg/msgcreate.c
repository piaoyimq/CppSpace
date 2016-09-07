#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main(int argc, char *argv[])
{
	int oflag=06440|IPC_CREAT;
	int msqid;
	key_t key;

	if(2!=argc)
	{
		printf("Usage:\n\t%s <pathname>\n",argv[1]);
		return 2;
	}

	key=ftok(argv[1], 0);
	if(key<0)
	{
		perror("ftok failed");
		return 1;
	}
	
	msqid=msgget(key, oflag);
	if(msqid<0)
	{
		perror("msgget failed");
		return 1;
	}
	return 0;

}


