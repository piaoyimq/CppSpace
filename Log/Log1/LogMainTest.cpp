/*
 * LogMainTest.cpp
 *
 *  Created on: Mar 26, 2016
 *      Author: piaoyimq
 */
#include <unistd.h>
#include "Log.h"

#define THREAD_NUMBER   10
#define COMPLEX_TEST     1




#if COMPLEX_TEST
void *f(void* args) {

    for (int i = 0; i < 10000; i++) {
//		sleep(1);
        App_Log(Log::Notice, Log::LAST_ID, "Function:%s, %d, line=%d", __FUNCTION__, 1, __LINE__);
        App_Log(Log::Notice, Log::LAST_ID, "Function:%s, %d, line=%d", __FUNCTION__, 2, __LINE__);
        App_Log(Log::Notice, Log::LAST_ID, "Function:%s,%d, line=%d", __FUNCTION__, 3, __LINE__);
    }
}
#endif


int main(int argc, char* argv[]) {


    printf("NAME_MAX=%d, PATH_MAX=%d\n", NAME_MAX, PATH_MAX);
    App_Log(Log::Notice, Log::LAST_ID, "_______Function:%s, %d, line=%d", __FUNCTION__, 1, __LINE__);
    App_Log(Log::Notice, Log::LAST_ID, "========Function:%s, %d, line=%d", __FUNCTION__, 2, __LINE__);

//    char a=getchar();
    Logging::instance().init("/home/ezhweib/CodeWorkSpace/CppSpace/Log", "my-test.log", 1024, 100000, 10);

    App_Log(Log::Notice, Log::LAST_ID, "Function:%s, %d, line=%d", __FUNCTION__, 3, __LINE__);

#if COMPLEX_TEST

    pthread_t id;
    int ret;

    for (int i = 0; i < THREAD_NUMBER; i++) {
        ret = pthread_create(&id, NULL, f, NULL);
        if (0 != ret) {
            App_Log(Log::Error, Log::LAST_ID, "Function:%s, pthread_create error", __FUNCTION__);
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < THREAD_NUMBER; i++) {
        ret = pthread_join(id, NULL);
        if (ret < 0) {
            App_Log(Log::Error, Log::LAST_ID, "Function:%s, pthread_join error", __FUNCTION__);
            exit(EXIT_FAILURE);
        }
    }
#endif

    return 0;
}

