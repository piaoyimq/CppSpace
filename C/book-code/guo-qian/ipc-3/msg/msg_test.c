#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct msgbuf
{
	long mtype;
	char mtext[1];
};

int main(int argc, char **argv)
{
	int	msqid;
	struct msqid_ds	info;
	struct msgbuf	buf;
//	struct msgbuf	buf;
	int ret;

	msqid=msgget(IPC_PRIVATE, 0664 | IPC_CREAT);
	if(msqid<0)
	{
		perror("msgget failed");
		return 1;
	}

	buf.mtype=1;
	buf.mtext[0]=1;

	ret=msgsnd(msqid, &buf,  1,  0);
	if(ret<0)
	{
		perror("msgsnd failed");
		return 1;
	}

	ret=msgctl(msqid, IPC_STAT, &info);
	printf("read-write: %03o, cbyte=%lu, qnum=%lu, gbytes=%lu\n", info.msg_perm.mode&0777, (unsigned long)info.msg_cbytes, (unsigned long)info.msg_qnum,(unsigned long)info.msg_qbytes);
	system("ipcs -q");

	ret=msgctl(msqid, IPC_RMID, NULL);
	if(ret<0)
	{
		perror("msgctl failed");
		return 1;
	}
	return 0;
}
