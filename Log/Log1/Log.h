/*
 * Log.h
 *
 *  Created on: Mar 29, 2016
 *      Author: piaoyimq
 */
#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <map>
#include <stdarg.h>   //va_start
#include <sys/syscall.h>
#include "BlockQueue.h"




class Log {
public:

    enum Level {
        Emergency = 0,  /* system is unusable */
        Alert = 1,      /* action must be taken immediately */
        Critical = 2,   /* critical conditions */
        Error = 3,      /* error conditions */
        Warning = 4,    /* warning conditions */
        Notice = 5,     /* normal but significant condition */
        Info = 6,       /* informational */
        Debug = 7,      /* debug-level messages */

        LAST /* This must be the last entry */
    };

    static Log& instance() {
        static Log _instance;    //Use static implement a instance.
        return _instance;
    }

    static pid_t getTid() {   // Must a static function, it called by a static funcion flushLogThread().
        return syscall(SYS_gettid);
    }

    static void *flushLogThread(void* args) {    //Must a static function, it would be called by pthread_create(), a function pointer point at it.
        printf("Log thread id: %d\n", getTid());
        Log::instance().async_write_log();
    }

    /* strings for printing message level */
    const char* getLogLevelString(const Log::Level logLevel) {
        const char* const logLevelString[] = { "***EMERG", "***ALERT", "***CRIT", "***ERROR", "!!!WARN", "+++NOTICE", "INFO", "DEBUG"};
        return (logLevel < (Log::Level) sizeof(logLevelString)) ? logLevelString[logLevel] : "*ERROR IN MESSAGE LEVEL*";
    }

    void writeLog(const Log::Level logLevel, int moduleId, const char* format, ...);

    void flush();

private:
    Log(); /*A private declaration for forbid inheriting*/
    ~Log();

    bool init(const char* file_name, int log_buf_size = 8192, int split_lines = 5000000, int max_queue_size = 0);

    void *async_write_log() {
        string single_log;
        while (m_log_queue->pop(single_log)) {
            pthread_mutex_lock(m_mutex);
            fputs(single_log.c_str(), m_fp);
            pthread_mutex_unlock(m_mutex);
        }
    }


    pid_t pid;
    map<pid_t, int> mapThread;
    pthread_mutex_t *m_mutex;
    char dirName[128];
    char logName[128];
    int splitLines;
    int logBufSize;
    long long counter;
    int m_today;
    FILE *m_fp;
    char *m_buf;
    BlockQueue<string> *m_log_queue;
    bool isAsync;
};


#define App_Log(logLevel, moduleId, ...)    Log::instance().writeLog(logLevel, moduleId, ##__VA_ARGS__);

#endif
