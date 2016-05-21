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
#include <sys/types.h>
#include <unistd.h>
#include <stdint.h>
#include <dirent.h>
#include "BlockQueue.h"
using namespace std;

#define STRING_LENGTH 20

#define DIR_LENGTH 200

#define NAME_LENGTH 60

#define BUF_SIZE 1024

#define ONE_LINE_LOG_LENGTH 1024

#define FILE_HEAD_LENGTH    1024

#define SHELL_CONTENT_LENGTH    2048

#define SPLIT_LINES  2000000

#define DEFAULT_TOTAL_LOG_SIZE	41 //0.09		//1024 //unit is Mbytes, this size is the total compressed logs size, not include the latest uncompressed log file size.

#define LOG_DIRECTORY   ".." //"/var/log/"

#if 1//TODO: would be move to common directory. (piaoyimq)
inline static pid_t getTid() {   // Must a static function, it called by a static funcion flushLogThread().
    return syscall(SYS_gettid);
}

inline void getNameByPid(pid_t pid, char *processName) {    //should become a un inline function after move to common.
    char procPidPath[DIR_LENGTH + 1] = { '\0' };
    char buf[BUF_SIZE] = { '\0' };
    sprintf(procPidPath, "/proc/%d/status", pid);
    FILE* fp = fopen(procPidPath, "r");
    if (NULL != fp) {
        if (fgets(buf, BUF_SIZE - 1, fp) == NULL) {
            fclose(fp);
        }
        fclose(fp);
        sscanf(buf, "%*s %s", processName);
    }
}

inline bool isDirPathExist(const char* dirpPath) {
    if (NULL == dirpPath || NULL == opendir(dirpPath)) {
        return false;
    }
    return true;
}
#endif

template<class T>
inline size_t getArrayLen(T& array) {
    return (sizeof(array) / sizeof(array[0]));
}

enum AppModuleId {
    LogId = 0, ConfId, LastId
};

const char logModuleString[][STRING_LENGTH] = { "Log", "Conf" };

enum Level {
    Emergency = 0, /* system is unusable */
    Alert, /* action must be taken immediately */
    Critical, /* critical conditions */
    Error, /* error conditions */
    Warning, /* warning conditions */
    Notice, /* normal but significant condition */
    Info, /* informational */
    Debug, /* debug-level messages */
    LastLevel
};

const char logLevelString[][STRING_LENGTH] = { "EMERG", "ALERT", "CRIT", "ERROR", "WARN", "NOTICE", "INFO", "DEBUG" };

class Log {
public:

    enum LogMethod {
        CMethod = 0, CmdMethod, CmdWithHeadMethod, CmdOnlyWriteHeadMethod
    };

    static Log& instance() {
        static Log _instance;
        return _instance;
    }  //Use static implement a instance.

//    void init(const char* dir, const char* fileName, uint32_t log_buf_size = ONE_LINE_LOG_LENGTH, uint32_t split_lines = SPLIT_LINES, uint32_t max_queue_size = 0);
    void init(const char* dir, const char* fileName, uint32_t oneLineLogSize, uint32_t split_lines, double logTotalSize, Level logLevel, uint32_t max_queue_size = 0);

    void writeLog(Level logLevel, AppModuleId moduleId, const char* format, ...);

private:

    Log(); /*A private declaration for forbid inheriting*/

    ~Log();

    void writeLogHead(char *logHead = NULL);

    size_t writeLogBody(Level logLevel, AppModuleId moduleId, char* des, size_t desLength, size_t offset, const char* format, va_list valst);

    bool moveLogs(const char* oldName, const char* newName, int32_t alreadyCompressFileAmount);

    void *async_write_log();

    //Must a static function, it would be called by pthread_create(), a function pointer point at it.
    static void *flushLogThread(void* args) {
        instance().async_write_log();
    }

    /* strings for printing message level */
    const char* getLogLevelString(Level logLevel) const {
        return logLevel < (static_cast<Level>(getArrayLen(logLevelString))) ? logLevelString[logLevel] : "Undefined";
    }

    /* strings for printing message level */
    const char* getLogModuleString(AppModuleId logModule) const {
        return logModule < (static_cast<AppModuleId>(getArrayLen(logModuleString))) ? logModuleString[logModule] : "Undefined";
    }

    void logfilesControl(int32_t alreadyCompressFileAmount);

    void flush() const;

    void logItself(LogMethod logMethod, Level logLevel, const char* format, ...);

    /* Attribute area */
    pid_t pid;

    char pidName[NAME_LENGTH + 1];

    map<pid_t, int> mapThread;

    pthread_mutex_t *m_mutex;

    char dirPath[DIR_LENGTH + 1];

    char logName[NAME_LENGTH + 1];   //default logName is "process name+'.log'"

    char logFullName[DIR_LENGTH + NAME_LENGTH + 2];

    uint32_t splitLines;

    uint32_t oneLineLogLength;

    uint64_t counter;

    int32_t currentLogAmount;

    double logFilesTotalSize;

    FILE* m_fp;

    char* m_buf;

    char logItselfBuf[BUF_SIZE];

    uint32_t logItselfLength;

    BlockQueue<string> *m_log_queue;

    bool isAsync;

    bool isStartup;

    uint64_t lostNum;

    Level enableLogLevel;   // enable the log level and the upper level
};

#define App_Log(logLevel, moduleId, ...)    Log::instance().writeLog(logLevel, moduleId, __VA_ARGS__)

#endif
