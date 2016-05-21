/*
 * LogMainTest.cpp
 *
 *  Created on: Mar 26, 2016
 *      Author: piaoyimq
 */
#include "Log.h"
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/stat.h>
//#include <pthread.h>

#define THREAD_NUMBER   1
#define COMPLEX_TEST     1



#if COMPLEX_TEST
void *f(void* args) {

    for (int i = 0; i < 10; i++) {
//		sleep(1);
        App_Log(Notice, LastId, "Function:%s, %d, line=%d", __FUNCTION__, 1, __LINE__);
        App_Log(Debug, LastId, "Function:%s, %d, line=%d", __FUNCTION__, 2, __LINE__);
        App_Log(Notice, LastId, "Function:%s,%d, line=%d", __FUNCTION__, 3, __LINE__);
    }
}
#endif


int main(int argc, char* argv[]) {
    App_Log(Notice, LastId, "_______Function:%s, %d, line=%d", __FUNCTION__, 1, __LINE__);

    App_Log(Debug, LastId, "========Function:%s, %d, line=%d", __FUNCTION__, 2, __LINE__);//will not logging it, because the default log level is notice.
    Log::instance().init(".", "my-test.log", 1024, 500000, 2, Debug, 100);
    sleep(4);//TODO: why if lack of sleep(3), it will lost logs. (piaoyimq)???

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

