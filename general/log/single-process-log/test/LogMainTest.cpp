/*
 * LogMainTest.cpp
 *
 *  Created on: Mar 26, 2016
 *      Author: piaoyimq
 */
#include "log/single-process-log/include/Log.h"
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/stat.h>

#define THREAD_NUMBER   10
#define COMPLEX_TEST     1

#if COMPLEX_TEST
void *f(void* args)
{
	static_cast<void>(args);
	for (int i = 0; i < 1000000; i++)
	{
		App_Log(Notice, LastId, "Function:%s, %d, line=%d", __FUNCTION__, 1, __LINE__);
		App_Log(Debug, LastId, "Function:%s, %d, line=%d", __FUNCTION__, 2, __LINE__);
		App_Log(Notice, LastId, "Function:%s,%d, line=%d", __FUNCTION__, 3, __LINE__);
	}
	return NULL;
}
#endif

int main(int argc, char* argv[])
{
	static_cast<void>(argc);
	static_cast<void>(argv);
	App_Log(Notice, LastId, "_______Function:%s, %d, line=%d", __FUNCTION__, 1, __LINE__);

	App_Log(Debug, LastId, "========Function:%s, %d, line=%d", __FUNCTION__, 2, __LINE__); //will not logging it, because the default log level is notice.
	Log::instance().init(".", "my-test.log", 1024, 500000, 2, Debug, 1000000);
// 	Log::instance().init(".", "my-test.log", 1024, 500000, 2, Debug, 0);

	App_Log(Notice, LastId, "Function:%s, %d, line=%d", __FUNCTION__, 3, __LINE__);

#if COMPLEX_TEST

	pthread_t id;
	int ret;

	for (int i = 0; i < THREAD_NUMBER; i++)
	{
		ret = pthread_create(&id, NULL, f, NULL);
		if (0 != ret)
		{
			App_Log(Error, LastId, "Function:%s, pthread_create error", __FUNCTION__);
			exit(EXIT_FAILURE);
		}
	}

	for (int i = 0; i < THREAD_NUMBER; i++)
	{
		ret = pthread_join(id, NULL);
		if (ret < 0)
		{
			App_Log(Error, LastId, "Function:%s, pthread_join error", __FUNCTION__);
			exit(EXIT_FAILURE);
		}
	}
#endif

	return 0;
}

