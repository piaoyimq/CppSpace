/*
 * LogMainTest.cpp
 *
 *  Created on: Mar 26, 2016
 *      Author: piaoyimq
 */
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include "Log.h"
#include <sys/stat.h>

#define THREAD_NUMBER   10
#define COMPLEX_TEST     1




#if COMPLEX_TEST
void *f(void* args) {

    for (int i = 0; i < 100; i++) {
//		sleep(1);
        App_Log(Notice, LastId, "Function:%s, %d, line=%d", __FUNCTION__, 1, __LINE__);
        App_Log(Notice, LastId, "Function:%s, %d, line=%d", __FUNCTION__, 2, __LINE__);
        App_Log(Notice, LastId, "Function:%s,%d, line=%d", __FUNCTION__, 3, __LINE__);
    }
}
#endif


int main(int argc, char* argv[]) {
//	char logFullNameNewTemp[50] = "my-test.log.1.gz";
//	printf("file=%s\n", logFullNameNewTemp);
//		struct stat statBuff;
//		stat(logFullNameNewTemp, &statBuff);
//		uint64_t fileSize=statBuff.st_size;
//		printf("one log size: %d\n", fileSize);
//
////		if(stat(logFullNameNewTemp, &statBuff)>0){
////			uint64_t fileSize=statBuff.st_size;
////			printf("one log size: %d\n", fileSize);
////		}
////		else
////		{
////			printf("no size\n");
////		}
//		exit(0);

    App_Log(Notice, LastId, "_______Function:%s, %d, line=%d", __FUNCTION__, 1, __LINE__);
    App_Log(Notice, LastId, "========Function:%s, %d, line=%d", __FUNCTION__, 2, __LINE__);

//    char a=getchar();
    Log::instance().init(".", "my-test.log", 1024, 100, 10);

    App_Log(Notice, LastId, "Function:%s, %d, line=%d", __FUNCTION__, 3, __LINE__);

#if COMPLEX_TEST

    pthread_t id;
    int ret;

    for (int i = 0; i < THREAD_NUMBER; i++) {
        ret = pthread_create(&id, NULL, f, NULL);
        if (0 != ret) {
            App_Log(Error, LastId, "Function:%s, pthread_create error", __FUNCTION__);
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < THREAD_NUMBER; i++) {
        ret = pthread_join(id, NULL);
        if (ret < 0) {
            App_Log(Error, LastId, "Function:%s, pthread_join error", __FUNCTION__);
            exit(EXIT_FAILURE);
        }
    }
#endif

    return 0;
}

