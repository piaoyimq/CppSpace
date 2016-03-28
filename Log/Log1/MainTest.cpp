/*
 * main.cc
 *
 *  Created on: 2016��3��26��
 *      Author: vicky
 */

#include <unistd.h>
#include "Log.h"


#define THREAD_NUMBER 100
#if 0
void *f(void* args)
{


	for(int i = 0;i < 5000; i++)
	{
//		sleep(1);
			Log::get_instance()->write_log(1, "d=%d,c=%c,s=%s,f=%f", i,'a',"log", 1.000);
			Log::get_instance()->write_log(2, "d=%d,c=%c,s=%s,f=%f", i,'a',"log", 1.000);
			Log::get_instance()->write_log(3, "d=%d,c=%c,s=%s,f=%f", i,'a',"log", 1.000);

			LOG_INFO("%d", 123456789);
			LOG_ERROR("%d", 123456789);
			LOG_DEBUG("%d", 123456789);
			LOG_WARN("%d", 123456789);
	}
}

#endif
int main()
{
	printf("Process id: %d\n", getpid());
	Log::get_instance()->init("./mylog.log", 100, 1000000, 10);//asynchronization model
// 	Log::get_instance()->init("./mylog.log", 100, 1000000, 0);//synchronization model
	sleep(2);
	int i = 0;
	App_Log(1, 2, "First log:%d", 3);
// 	Log::get_instance()->write_log(1, 2, "d=%d,c=%c,s=%s,f=%f", i,'a',"log", 1.000);
#if 0
	Log::get_instance()->write_log(2, "d=%d,c=%c,s=%s,f=%f", i,'a',"log", 1.000);
	Log::get_instance()->write_log(3, "d=%d,c=%c,s=%s,f=%f", i,'a',"log", 1.000);
	LOG_INFO("%d", 123456789);
	LOG_ERROR("%d", 123456789);
	LOG_DEBUG("%d", 123456789);
	LOG_WARN("%d", 123456789);

	pthread_t id;
	int ret;


	for(int i = 0; i < THREAD_NUMBER; i++)
	{
		ret=pthread_create(&id, NULL, f, NULL);
		if(0!=ret){
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

// 	for(int i =0;i<THREAD_NUMBER;i++){
// 		ret=pthread_join(id,NULL);
// 			if(ret < 0){
// 				printf("pthread_join error\n");
// 				exit(1);
// 			}
// 	}

	//for(;;)
	{
//		sleep(15);
		Log::get_instance()->flush();
	}
	#endif
	return 0;
}

