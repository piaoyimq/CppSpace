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

#define DIR_LENGTH 200

#define NAME_LENGTH 60

#define BUF_SIZE 1024

#define ONE_LINE_LOG_LENGTH 1024

#define FILE_HEAD_LENGTH    1024

#define SHELL_CONTENT_LENGTH    2048

#define SPLIT_LINES  2000000

#define LOG_DIRECTORY   "/home/ezhweib/CodeWorkSpace/CppSpace/Log/Log1" //"/var/log/"





#if 1//TODO: would be move to common directory. (piaoyimq)
inline static pid_t getTid() {   // Must a static function, it called by a static funcion flushLogThread().
    return syscall(SYS_gettid);
}


inline void getNameByPid(pid_t pid, char *processName) {    //should become a un inline function after move to common.
    char procPidPath[DIR_LENGTH+1]={'\0'};
    char buf[BUF_SIZE]={'\0'};
    sprintf(procPidPath, "/proc/%d/status", pid);
    FILE* fp = fopen(procPidPath, "r");
    if(NULL != fp){
        if( fgets(buf, BUF_SIZE-1, fp)== NULL ){
            fclose(fp);
        }
        fclose(fp);
        sscanf(buf, "%*s %s", processName);
    }
}


inline bool isDirPathExist(const char* dirpPath){
    if(NULL == dirpPath || NULL == opendir(dirpPath)){
        return false;
    }

    return true;
}
#endif

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
    
    enum LogMethod{
        C_METHOD= 0,
        COMMAND_METHOD,
    };

    static Logging& instance() { static Logging _instance;  return _instance;}  //Use static implement a instance.

    void init(const char* dirPath, const char* fileName, uint32_t log_buf_size = ONE_LINE_LOG_LENGTH, uint32_t split_lines = SPLIT_LINES, uint32_t max_queue_size = 0);

    void writeLog(Log::Level logLevel, Log::AppModuleID moduleId, const char* format, ...);

private:

    Logging(); /*A private declaration for forbid inheriting*/

    ~Logging();

    void moveLogs(const char* oldName, const char* newName, uint32_t alreadyCompressFileAmount);

    void *async_write_log() const;

    //Must a static function, it would be called by pthread_create(), a function pointer point at it.
    static void *flushLogThread(void* args) { printf("Logging thread id: %d\n", getTid()); instance().async_write_log();}

    /* strings for printing message level */
    const char* getLogLevelString(Log::Level logLevel) const {return logLevel < (static_cast<Log::Level>(Log::getArrayLen(Log::logLevelString))) ? Log::logLevelString[logLevel] : "Undefined";}

    /* strings for printing message level */
    const char* getLogModuleString(Log::AppModuleID logModule) const { return logModule < (static_cast<Log::AppModuleID>(Log::getArrayLen(Log::logModuleString))) ? Log::logModuleString[logModule] : "Undefined";}

    void logFileCompression(uint32_t fileAmount) ;

    void flush()const;

    void logItself(LogMethod logMethod, Log::Level logLevel, const char* format, ...);


    /* Attribute area */
    pid_t pid;

    char pidName[NAME_LENGTH+1];

    map<pid_t, int> mapThread;

    pthread_mutex_t *m_mutex;

    char dirPath[DIR_LENGTH+1];

    char logName[NAME_LENGTH+1];   //default logName is "process name+'.log'"

    char logFullName[DIR_LENGTH+NAME_LENGTH+2];

    uint32_t splitLines;

    uint32_t oneLineLogLength;

    uint64_t counter;

    uint32_t currentLogAmount;

    FILE* m_fp;

    char* m_buf;
    
    char logItselfBuff[BUF_SIZE];

    BlockQueue<string> *m_log_queue;

    bool isAsync;
};


#define App_Log(logLevel, moduleId, ...)    Logging::instance().writeLog(logLevel, moduleId, __VA_ARGS__)

#endif
