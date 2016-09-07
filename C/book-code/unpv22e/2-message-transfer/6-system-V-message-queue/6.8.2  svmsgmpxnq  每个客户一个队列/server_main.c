#include "../../6-system-V-message-queue/6.8  svmsgmpx1q  复用消息/svmsg.h"

void	server(int, int);

int
main(int argc, char **argv)
{
	int		msqid;

	msqid = Msgget(MQ_KEY1, SVMSG_MODE | IPC_CREAT);

	server(msqid, msqid);	/* same queue for both directions */

	exit(0);
}
