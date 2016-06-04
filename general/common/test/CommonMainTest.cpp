/*
 * CommonMainTest.cpp
 *
 *  Created on: 2016-05-21
 *      Author: piaoyimq
 */

#include "common/include/Common.h"
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
	static_cast<void>(argc);
	static_cast<void>(argv);
	char task_name[50];
	pid_t pid = getpid();
	printf("pid of this process:%d\n", pid);
	getNameByPid(pid, task_name);

	/*
	 strcpy(task_name, argv[0]+2);
	 printf("task name is %s\n", task_name);
	 getPidByName(task_name);
	 */
	printf("task name is %s\n", task_name);
	getPidByName(task_name);
	return EXIT_SUCCESS;
}
