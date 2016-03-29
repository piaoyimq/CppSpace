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

    static Log& get_instance(){
        static Log instance;    //Use static implement a instance.
        return instance;
    }

    static pid_t gettid() {   // Must a static function, it called by a static funcion flush_log_thread().
        return syscall(SYS_gettid);
    }

    static void *flush_log_thread(void* args) {    //Must a static function, it would be called by pthread_create(), a function pointer point at it.
        printf("Log thread id: %d\n", gettid());
        Log::get_instance().async_write_log();
    }

    /* strings for printing message level */
    const char* getLogLevelString(const Log::Level logLevel) {
        const char* const logLevelString[] = { "***EMERG", "***ALERT", "***CRIT", "***ERROR", "!!!WARN", "+++NOTICE", "INFO", "DEBUG"};
        return (logLevel < (Log::Level) sizeof(logLevelString)) ? logLevelString[logLevel] : "*ERROR IN MESSAGE LEVEL*";
    }

    void write_log(const Log::Level logLevel, int moduleId, const char* format, ...);

    void flush(void);

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
    char dir_name[128];
    char log_name[128];
    int m_split_lines;
    int m_log_buf_size;
    long long m_count;
    int m_today;
    FILE *m_fp;
    char *m_buf;
    block_queue<string> *m_log_queue;
    bool m_is_async;
};


#define App_Log(logLevel, moduleId, ...)    Log::get_instance().write_log(logLevel, moduleId, ##__VA_ARGS__);

#endif
