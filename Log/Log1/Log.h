/*
 * log.h
 *
 *  Created on: 2016年3月26日
 *      Author: vicky
 */

#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <map>
#include <iostream>
#include <string>
#include <stdarg.h>
#include <pthread.h>
#include <sys/syscall.h>
#include "BlockQueue.h"
using namespace std;

inline pid_t gettid()
{
     return syscall(SYS_gettid);  /*这才是内涵*/
}

class Log
{
	public:
		static Log* get_instance()	//Why not return a reference?
		{
			static Log instance;	//Use static implement a instance.
			return &instance;
		}

		static void *flush_log_thread(void* args)
		{
			printf("Tid=%d\n", gettid());
			Log::get_instance()->async_write_log();
		}

		bool init(const char* file_name, int log_buf_size = 8192, int split_lines = 5000000, int max_queue_size = 0);

		void write_log(int level, const char* format, ...);

		void flush(void);

	private:
		Log();
		virtual ~Log();
		void *async_write_log()
		{
			string single_log;
			while(m_log_queue->pop(single_log))
			{
				pthread_mutex_lock(m_mutex);
				fputs(single_log.c_str(), m_fp);
				pthread_mutex_unlock(m_mutex);
			}
		}

	private:
		pid_t pid;
		map<pid_t, int> mapThread;
		pthread_mutex_t *m_mutex;
		char dir_name[128];
		char log_name[128];
		int m_split_lines;
		int m_log_buf_size;
		long long  m_count;
		int m_today;
		FILE *m_fp;
		char *m_buf;
		block_queue<string> *m_log_queue;
		bool m_is_async;
};

#define LOG_DEBUG(format, ...) Log::get_instance()->write_log(0, format, __VA_ARGS__)
#define LOG_INFO(format, ...) Log::get_instance()->write_log(1, format, __VA_ARGS__)
#define LOG_WARN(format, ...) Log::get_instance()->write_log(2, format, __VA_ARGS__)
#define LOG_ERROR(format, ...) Log::get_instance()->write_log(3, format, __VA_ARGS__)

#endif
