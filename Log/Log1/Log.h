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
#include <dirent.h>
#include "BlockQueue.h"

#define STRING_LENGTH 20

#define BUF_SIZE 1024

#define ONE_LINE_LOG_LENGTH 1024

#define FILE_HEAD_LENGTH    1024

#define SHELL_CONTENT_LENGTH    8192

#define SPLIT_LINES  2000000

#define LOG_DIRECTORY   "/home/coding/workspace/CppSpace/Log/Log1" //"/var/log/"





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


const char logModuleString[][STRING_LENGTH] = {
    "Log",
    "Conf"
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


const char logLevelString[][STRING_LENGTH] = {
    "EMERG",
    "ALERT",
    "CRIT",
    "ERROR",
    "WARN",
    "NOTICE",
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
        return logLevel < (static_cast<Log::Level>(Log::getArrayLen(Log::logLevelString))) ? Log::logLevelString[logLevel] : "Undefined";
    }

    /* strings for printing message level */
    const char* getLogModuleString(Log::AppModuleID logModule) {
        return logModule < (static_cast<Log::AppModuleID>(Log::getArrayLen(Log::logModuleString))) ? Log::logModuleString[logModule] : "Undefined";
    }

    void logFileCompression(uint32_t fileAmount);

    void writeLog(Log::Level logLevel, Log::AppModuleID moduleId, const char* format, ...);

    void flush();

    void init(const char* dirPath, const char* fileName, int log_buf_size = ONE_LINE_LOG_LENGTH, int split_lines = SPLIT_LINES, int max_queue_size = 0);
private:
    Logging(); /*A private declaration for forbid inheriting*/
    ~Logging();
    void moveLogs(const char* oldName, const char* newName, int alreadyCompressFileAmount);

    void *async_write_log() {
        string single_log;
        while (m_log_queue->pop(single_log)) {
            pthread_mutex_lock(m_mutex);
            fputs(single_log.c_str(), m_fp);
            pthread_mutex_unlock(m_mutex);
        }
    }


    pid_t pid;
    char pidName[NAME_MAX+1];
    map<pid_t, int> mapThread;
    pthread_mutex_t *m_mutex;
    char dirPath[PATH_MAX+1];
    char logName[NAME_MAX+1];   //default logName is "process name+'.log'"
    char logFullName[PATH_MAX+NAME_MAX+1];
    int splitLines;
    int oneLineLogLength;
    long long counter;
    int currentLogAmount;
    FILE *m_fp;
    char *m_buf;
    BlockQueue<string> *m_log_queue;
    bool isAsync;
};


#define App_Log(logLevel, moduleId, ...)    Logging::instance().writeLog(logLevel, moduleId, __VA_ARGS__)

#endif
