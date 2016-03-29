/*
 * Log.h
 *
 *  Created on: Mar 26, 2016
 *      Author: piaoyimq
 */
#include <unistd.h>
#include "Log.h"

#define THREAD_NUMBER   5
#define COMPLEX_TEST     1





#if COMPLEX_TEST
void *f(void* args) {

    for (int i = 0; i < 100000; i++) {
//		sleep(1);
        App_Log(Log::Notice, 2, "First log:%d", 3);
        App_Log(Log::Notice, 2, "First log:%d", 3);
        App_Log(Log::Notice, 2, "First log:%d", 3);
        App_Log(Log::Notice, 2, "First log:%d", 3);
    }
}
#endif


int main() {
    App_Log(Log::Notice, 2, "First log:%d", 3);
    App_Log(Log::Notice, 2, "First log:%d", 3);
    App_Log(Log::Notice, 2, "First log:%d", 3);
    App_Log(Log::Notice, 2, "First log:%d", 3);

#if COMPLEX_TEST

    pthread_t id;
    int ret;

    for (int i = 0; i < THREAD_NUMBER; i++) {
        ret = pthread_create(&id, NULL, f, NULL);
        if (0 != ret) {
            printf("pthread_create error\n");
            exit(1);
        }
    }

    for (int i = 0; i < THREAD_NUMBER; i++) {
        ret = pthread_join(id, NULL);
        if (ret < 0) {
            printf("pthread_join error\n");
            exit(1);
        }
    }
#endif

    return 0;
}

