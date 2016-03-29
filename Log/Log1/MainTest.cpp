/*
 * Log.h
 *
 *  Created on: Mar 26, 2016
 *      Author: piaoyimq
 */

#include <unistd.h>
#include "Log.h"

#define THREAD_NUMBER   5
#define SIMPLE_TEST     1

#if SIMPLE_TEST
void *f(void* args) {

    for (int i = 0; i < 1; i++) {
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

#if SIMPLE_TEST

    pthread_t id;
    int ret;

    for (int i = 0; i < THREAD_NUMBER; i++) {
        ret = pthread_create(&id, NULL, f, NULL);
        if (0 != ret) {
            printf("pthread_create error\n");
            exit(1);
        }
//		printf("thread id=%u\n", id);
//		ret=pthread_join(id,NULL);
//					if(ret < 0){
//						printf("pthread_join error\n");
//						exit(1);
//					}
    }

    for (int i = 0; i < THREAD_NUMBER; i++) {
        ret = pthread_join(id, NULL);
        if (ret < 0) {
            printf("pthread_join error\n");
            exit(1);
        }
    }

    //for(;;)
    {
//		sleep(15);
//        Log::get_instance()->flush();
    }
#endif

    return 0;
}

