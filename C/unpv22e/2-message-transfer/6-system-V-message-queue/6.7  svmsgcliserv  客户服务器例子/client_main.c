#include "../../6-system-V-message-queue/6.7  svmsgcliserv  客户服务器例子/svmsg.h"

void	client(int, int);

int
main(int argc, char **argv)
{
	int		readid, writeid;

		/* 4assumes server has created the queues */
	writeid = Msgget(MQ_KEY1, 0);
	readid = Msgget(MQ_KEY2, 0);

	client(readid, writeid);

		/* 4now we can delete the queues */
	Msgctl(readid, IPC_RMID, NULL);
	Msgctl(writeid, IPC_RMID, NULL);

	exit(0);
}
