/* include mesg_send */
#include "../../6-system-V-message-queue/6.8  svmsgmpx1q  复用消息/mesg.h"

ssize_t
mesg_send(int id, struct mymesg *mptr)
{
	return(msgsnd(id, &(mptr->mesg_type), mptr->mesg_len, 0));
}
/* end mesg_send */

void
Mesg_send(int id, struct mymesg *mptr)
{
	ssize_t	n;

	if ( (n = msgsnd(id, &(mptr->mesg_type), mptr->mesg_len, 0)) == -1)
		err_sys("mesg_send error");
}
