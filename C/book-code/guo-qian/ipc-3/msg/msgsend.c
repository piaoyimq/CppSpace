#include <stdlib.h>
#include <string.h>
#include "msg_common.h"

int main(int argc, char *argv[])
{
	int len;
	int msqid;
	int key;
	int ret;
	
	struct msgbuf *buffer;

	if(4!=argc)
	{
		printf("Usage:\n\t%s <pathname> <length> <type>\n", argv[0]);
		return 2;
	}

	len=atoi(argv[2]);

	key=ftok(argv[1],0);
	if(key<0)
	{
		perror("ftok error");
		return 1;
	}

	msqid=msgget(key, O_WRONLY);
	if(msqid<0)
	{
		perror("msgget failed");
		return 1;
	}

	buffer=(struct msgbuf *)malloc(len+sizeof(long));
	if(!buffer)
	{
		printf("malloc failed\n");
		return 1;
	}

	memset(buffer, 1, len+sizeof(long));
	buffer->mtype=atol(argv[3]);
	ret=msgsnd(msqid, buffer,  len, 0);
	if(ret<0)
	{
		perror("msgsnd failed\n");
		goto exit;
	}
exit:
	free(buffer);
	return 0;
}
