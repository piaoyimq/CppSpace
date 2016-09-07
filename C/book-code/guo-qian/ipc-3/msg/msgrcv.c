#include "msg_common.h"

int main(int argc, char *argv[])
{
	int msqid;
	long type;
	int key;
	int ret;
	
	char *buffer[MSG_BUF_MAX_LENGTH];
	struct msgbuf *ptr=(struct msgbuf *)buffer;

	if(3!=argc)
	{
		printf("Usage:\n\t%s <pathname> <type>\n", argv[0]);
		return 2;
	}

	key=ftok(argv[1],0);
	if(key<0)
	{
		perror("ftok error");
		return 1;
	}

	msqid=msgget(key, O_RDONLY);
	if(msqid<0)
	{
		perror("msgget failed");
		return 1;
	}

	memset(buffer, 0, MSG_BUF_MAX_LENGTH);
	
	type=atoi(argv[2]);
	ret=msgrcv(msqid, ptr,  MSG_BUF_MAX_LENGTH, type, 0);
	if(ret<0)
	{
		perror("msgrcv failed");
		return 1;
	}
	printf("read %d bytes:  type=%ld\n", ret, ptr->mtype);

	return 0;
}

