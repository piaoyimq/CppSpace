/* include mesg_send */
#include "../../4-pipi-fifo/4.10  pipemesg  字节流与消息/mesg.h"

ssize_t
mesg_send(int fd, struct mymesg *mptr)
{
	return(write(fd, mptr, MESGHDRSIZE + mptr->mesg_len));
}
/* end mesg_send */

void
Mesg_send(int fd, struct mymesg *mptr)
{
	ssize_t	n;

	if ( (n = mesg_send(fd, mptr)) != mptr->mesg_len)
		err_quit("mesg_send error");
}
