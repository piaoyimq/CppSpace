#include "../../6-system-V-message-queue/6.8  svmsgmpx1q  复用消息/svmsg.h"

void	client(int, int);

int
main(int argc, char **argv)
{
	int		msqid;

		/* 4server must create the queue */
	msqid = Msgget(MQ_KEY1, 0);

	client(msqid, msqid);	/* same queue for both directions */

	exit(0);
}
