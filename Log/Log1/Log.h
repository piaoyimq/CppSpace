/*
 * Logging.h
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
#include <unistd.h>
#include <stdint.h>
#include "BlockQueue.h"



namespace Log{

template <class T>
inline size_t getArrayLen(T& array){
    return (sizeof(array) / sizeof(array[0]));
}


enum AppModuleID{
    LOG_ID = 0,
    CONF_ID,
    LAST_ID
};


const char logModuleString[][16] = {
    "<Logging>",
    "<Conf>"
};


enum Level {
    Emergency = 0,  /* system is unusable */
    Alert,      /* action must be taken immediately */
    Critical,   /* critical conditions */
    Error,      /* error conditions */
    Warning,    /* warning conditions */
    Notice,     /* normal but significant condition */
    Info,       /* informational */
    Debug,      /* debug-level messages */
    LAST
};


const char logLevelString[][16] = {
    "***EMERG",
    "***ALERT",
    "***CRIT",
    "***ERROR",
    "!!!WARN",
    "+++NOTICE",
    "INFO",
    "DEBUG"
};
}


class Logging {
public:



    static Logging& instance() {
        static Logging _instance;    //Use static implement a instance.
        return _instance;
    }

    static pid_t getTid() {   // Must a static function, it called by a static funcion flushLogThread().
        return syscall(SYS_gettid);
    }

    static void *flushLogThread(void* args) {    //Must a static function, it would be called by pthread_create(), a function pointer point at it.
        printf("Logging thread id: %d\n", getTid());
        Logging::instance().async_write_log();
    }

    /* strings for printing message level */
    const char* getLogLevelString(Log::Level logLevel) {
        return logLevel < (static_cast<Log::Level>(Log::getArrayLen(Log::logLevelString))) ? Log::logLevelString[logLevel] : "Undefined level";
    }

    /* strings for printing message level */
    const char* getLogModuleString(Log::AppModuleID logModule) {
        return logModule < (static_cast<Log::AppModuleID>(Log::getArrayLen(Log::logModuleString))) ? Log::logModuleString[logModule] : "Undefined module";
    }

    void logFileCompression(const char* fileName, uint32_t fileAmount);

    void writeLog(Log::Level logLevel, Log::AppModuleID moduleId, const char* format, ...);

    void flush();

private:
    Logging(); /*A private declaration for forbid inheriting*/
    ~Logging();

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
    int currentLogAmount;
    FILE *m_fp;
    char *m_buf;
    BlockQueue<string> *m_log_queue;
    bool isAsync;
};


#define App_Log(logLevel, moduleId, ...)    Logging::instance().writeLog(logLevel, moduleId, ##__VA_ARGS__);

#endif
