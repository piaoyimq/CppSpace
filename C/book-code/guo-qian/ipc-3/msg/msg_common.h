#ifndef _MSG_COMMON_H_
#define _MSG_COMMON_H_

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#define MSG_MAX_LENGTH		1024
#define MSG_BUF_MAX_LENGTH	(MSG_MAX_LENGTH+sizeof(long))

struct msgbuf
{
	long mtype;
	char mtext[1];	
};

#endif
