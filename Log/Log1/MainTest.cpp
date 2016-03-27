/*
 * main.cc
 *
 *  Created on: 2016Äê3ÔÂ26ÈÕ
 *      Author: vicky
 */

#include <unistd.h>
#include "Log.h"


#define THREAD_NUMBER 2

void *f(void* args)
{


	for(int i = 0;i < 10; i++)
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


int main()
{
	printf("Process id: %d\n", getpid());
//	Log::get_instance()->init("./mylog.log", 10000, 200000, 100);//asynchronization model
	Log::get_instance()->init("./mylog.log", 100, 200000, 0);//synchronization model
	sleep(1);
	int i = 0;
	Log::get_instance()->write_log(1, "d=%d,c=%c,s=%s,f=%f", i,'a',"log", 1.000);
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

	for(int i =0;i<THREAD_NUMBER;i++){
		ret=pthread_join(id,NULL);
			if(ret < 0){
				printf("pthread_join error\n");
				exit(1);
			}
	}


	//for(;;)
	{
//		sleep(15);
		Log::get_instance()->flush();
	}
	return 0;
}


