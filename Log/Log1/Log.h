/*
 * log.h
 *
 *  Created on: 2016��3��26��
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



namespace LogN{
#if 1
enum Level {
    Emergency = 0,    /* system is unusable */
    Alert = 1,        /* action must be taken immediately */
    Critical = 2,     /* critical conditions */
    Error = 3,        /* error conditions */
    Warning = 4,      /* warning conditions */
    Notice = 5,       /* normal but significant condition */
    Info = 6,         /* informational */
    Debug = 7,        /* debug-level messages */

    LAST              /* This must be the last entry */
};


/* strings for printing message level */
static __inline__ const char* getLogLevelString(const LogN::Level logLevel)
{
    const char* const logLevelString[] =
    {
        "***EMERG",
        "***ALERT",
        "***CRIT",
        "***ERROR",
        "!!!WARN",
        "+++NOTICE",
        "INFO",
        "DEBUG",
    };

    return (logLevel < (LogN::Level)sizeof(logLevelString)) ? logLevelString[logLevel] : "*ERROR IN MESSAGE LEVEL*";
}
}
#endif
inline pid_t gettid()
{
     return syscall(SYS_gettid);  /*������ں�*/
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

		void write_log(int level, int moduleId, const char* format, ...);

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


#if 1

// void Logging_log(const Log::Level_t logLevel, int moduleId/*const App_ModuleId_t moduleId*/,
//         const char* format, ...)
// {
//     // if (Logging_isLogEnabled(moduleId, logLevel))
//     {
//         va_list argList;
//         va_start(argList, format);
//         Log::get_instance()->write_log(logLevel, moduleId, format, __VA_ARGS__);
//         // logging.log(logLevel,moduleId, NULL, 0,
//                 // LOG_FACILITY_DEFAULT, format, argList);
//         va_end(argList);
//     }
// }

#define App_Log LOG
#define LOG(logLevel, moduleId, ...)                                          \
    do {                                                                      \
        {                                                                     \
            Log::get_instance()->write_log(logLevel, moduleId, ##__VA_ARGS__);  \
        }                                                                     \
    } while (0)


// App_Log(Log::Notice, S6B_ID, "%s: unknown counter type: %d", __FUNCTION__, type);
#endif


#define     LOG_DEBUG(format, ...)      Log::get_instance()->write_log(0, format, __VA_ARGS__)
#define     LOG_INFO(format, ...)       Log::get_instance()->write_log(1, format, __VA_ARGS__)
#define     LOG_WARN(format, ...)       Log::get_instance()->write_log(2, format, __VA_ARGS__)
#define     LOG_ERROR(format, ...)      Log::get_instance()->write_log(3, format, __VA_ARGS__)

// }

#endif
