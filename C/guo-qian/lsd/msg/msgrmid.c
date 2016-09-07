#include "msg_common.h"

int main(int argc, char *argv[])
{
	int msqid;
	int key;
	int ret;
	if(argc!=2)
	{
		printf("Usage:\n\t%s <pathname>\n", argv[0]);
		return 2;
	}
	key=ftok(argv[1], 0);
	if(key<0)
	{
		perror("ftok failed");
		return 1;
	}

	msqid=msgget(key, O_RDWR);
	if(msqid<0)
	{
		perror("msgget failed");
		return 1;
	}

	ret=msgctl(msqid, IPC_RMID, NULL);
	if(ret<0)
	{
		perror("msgctl failed");
		return 1;
	}
	return  0;
}
