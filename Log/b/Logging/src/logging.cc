/*---------------------------------------------------------------------------------------------------------------------
 * Copyright (C) 2000-2015 by Ericsson AB
 *
 * Address:
 * Torshamnsgatan 21
 * 164 83 Kista
 * SWEDEN
 *
 * Telephone: +46 10 719 00 00
 *
 * All Rights Reserved. No part of this software may be reproduced in
 * any form without the written permission of the copyright owner.
 *-------------------------------------------------------------------------------------------------------------------*/

#include "app/Logging/include/logging.h"
#include "app/Logging/platform/include/log.h"
#include "srvlib/include/atomic.h"
#include "srvlib/include/backtrace.h"
#include "srvlib/include/srvutil.h"
#include "srvlib/include/rwmutex.h"
#include "logging_syslog.h"
#include "logging_memory.h"

#include "Logging/SequenceNumberGenerator.h"

#include <assert.h>
#include <ctype.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

/* ------------------------------------------------------------------------- */
/* INTERNAL FORWARD DEFINITIONS                                              */
/* ------------------------------------------------------------------------- */
static struct timeval Logging_getTimeOfDay_Default();

/* ------------------------------------------------------------------------- */
/* INTERNAL DEFINITIONS                                                      */
/* ------------------------------------------------------------------------- */

/** BACKTRACE_IN_SYSLOG:
 * A backtrace for ERROR level and above is always generated and included
 * in our memory-logs.
 *
 * @param BACKTRACE_IN_SYSLOG   true    Include backtrace in syslog.
 *                              false   Don't include backtrace in syslog. */
#define BACKTRACE_IN_SYSLOG true

#define LOG_BUFFER_SIZE     (256 * 1024)
struct RingBuffer_t* logBuffer;

/* ------------------------------------------------------------------------- */
/* GLOBAL DATA                                                               */
/* ------------------------------------------------------------------------- */

/* This is located outside Logging class because we want IsLoggingEnabled()
 * to be __inlined__ for hi speed purposes. */
struct LogItem_t logItemList[LAST_ID];

/* Interfaces */
Logging_GetTimeofdayFn_t Logging_getTimeofdayFn = Logging_getTimeOfDay_Default;

/* ------------------------------------------------------------------------- */
/* Class definition: Logging                                                 */
/* ------------------------------------------------------------------------- */
class Logging
{
private:
    /* Identifies logging entity within board (pid) and provides context for
     * those reading log (process name). This is passed as identity when
     * opening syslog, except on SSR where it is used at start of all printouts.
     * Syslog does not copy the string, so we need to keep the buffer.
     * Fixed, set before logging starts. */
    char identityString[32];

    /* Application-specific prefix, to be appended after identity string.
     * Variable, will change during application's life time
     * (therefore protected by mutex). */
    std::string     logPrefix;
    srvlib::RwMutex logPrefixMutex;

public:
    /* Constructor */
    Logging() :
        logPrefix(),
        logPrefixMutex()
    {
        memset((char*)logItemList, 0, sizeof(logItemList));

        /* Init identity string in case application tries to log before
         * Logging_init() has been called. */
        Srv_Snprintf(identityString, sizeof(identityString), "[%d]", getpid());

        Logging_memory_init();
        logBuffer = Logging_memory_create(LOG_BUFFER_SIZE);

        if (Log_Init())
        {
            openSyslog();
        }
    }

    /* Destructor */
    ~Logging()
    {
        for (size_t i = 0; i < Array_Size(logItemList); ++i)
        {
            Logging_memory_destroy(&logItemList[i].internalMemoryBuffer,
                    logItemList[i].internalMemorySize);
        }

        Logging_memory_destroy(&logBuffer, LOG_BUFFER_SIZE);

        Log_Shutdown();
        Logging_Syslog_close();
    }

private:
    Log::SequenceNumber getNewSequenceNumber(void);
    void openSyslog(void);

public:
    void setIdentityString(const std::string& str);
    void setLogPrefix(const std::string& str);
    Log::Mask_t setLogMask(const App_ModuleId_t moduleId, const Log::Mask_t logMask,
            const char* name, const int internalMemoryBufferSize);
    Log::Mask_t getLogMask(const App_ModuleId_t moduleId) const;
    const char* getLogName(const App_ModuleId_t moduleId) const;

    void log(const Log::Level_t logLevel,
            const App_ModuleId_t moduleId,
            const char* const fileName,
            const int32_t lineNumber,
            const int facility,
            const char* const format,
            va_list argList) __attribute__ ((format(printf, 7, 0)));
};

/* ------------------------------------------------------------------------- */
/* INTERNAL FUNCTIONS                                                        */
/* ------------------------------------------------------------------------- */
static struct timeval Logging_getTimeOfDay_Default()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv;
}

/* ------------------------------------------------------------------------- */
/* Class functions: Logging                                                  */
/* ------------------------------------------------------------------------- */
void Logging::openSyslog()
{
    /* Open or re-open connection towards syslog service */
    Logging_Syslog_open(identityString, LOG_FACILITY_DEFAULT);
}

void Logging::setIdentityString(const std::string& str)
{
    Srv_Strncpy(identityString, str.c_str(), sizeof(identityString));

    openSyslog();
}

void Logging::setLogPrefix(const std::string& str)
{
    logPrefixMutex.lockWrite();
    logPrefix = str;
    logPrefixMutex.unlock();
}

#if 0
static void Logging_pushLogMask(const App_ModuleId_t moduleId, const Log::Mask_t logMask)
{
    assert(moduleId < LAST_ID);

    /* copy: [0|->[1]->[2]->[3]--->[n-1]; n = stack-size */
    for (size_t i = Array_Size(logItemList[moduleId].logMask_) - 1; i > 0; --i)
    {
        logItemList[moduleId].logMask_[i] = logItemList[moduleId].logMask_[i - 1];
    }
    logItemList[moduleId].logMask_[0] = logMask;
}

static void Logging_popLogMask(const App_ModuleId_t moduleId)
{
    assert(moduleId < LAST_ID);

    /* copy: [0|<-[1]<-[2]<-[3]<---[n]; n = stack-size */
    for (size_t i = 0; i < Array_Size(logItemList[moduleId].logMask_) - 1; ++i)
    {
        logItemList[moduleId].logMask_[i] = logItemList[moduleId].logMask_[i + 1];
    }
}

void Logging::pushLogMask(const App_ModuleId_t moduleId, const Log::Mask_t logMask)
{
    assert(moduleId <= LAST_ID);
    if (moduleId == LAST_ID)
    {
        for (App_ModuleId_t x = (App_ModuleId_t)0; x < LAST_ID; ++x)
        {
            Logging_pushLogMask(x, logMask);
        }
    }
    else
    {
        Logging_pushLogMask(moduleId, logMask);
    }
}

void Logging::popLogMask(const App_ModuleId_t moduleId)
{
    assert(moduleId <= LAST_ID);
    if (moduleId == LAST_ID)
    {
        for (App_ModuleId_t x = (App_ModuleId_t)0; x < LAST_ID; ++x)
        {
            Logging_popLogMask(x);
        }
    }
    else
    {
        Logging_popLogMask(moduleId);
    }
}
#endif

Log::Mask_t Logging::setLogMask(const App_ModuleId_t moduleId, const Log::Mask_t logMask,
        const char* name, const int internalMemoryBufferSize)
{
    assert(moduleId < LAST_ID);
    const Log::Mask_t result = getLogMask(moduleId);

    if (name)
    {
        /* if the name contains "_ID" then this is because it was initialized
         * from a unit-test executable.
         * Remove "_ID" from the name and only use the module-name. */
        const char*  ptr     = Srv_Strstr(name, "_ID");
        const size_t nameLen = (ptr == NULL) ? sizeof(logItemList[moduleId].name_) :
                               std::min(sizeof(logItemList[moduleId].name_), ((size_t)(ptr - name) + 1));
        Srv_Strncpy(logItemList[moduleId].name_, name, nameLen);
    }
    else if (logItemList[moduleId].name_[0] == '\0')
    {
        /* nothing is set yet and nothing is provided. This should have been
         * done directly at startup. Perhaps this is a unit/component test running */
        Srv_Strncpy(logItemList[moduleId].name_, "????", sizeof(logItemList[moduleId].name_));
    }

    for (unsigned int i = 0; i < Array_Size(logItemList[moduleId].logMask_); ++i)
    {
        logItemList[moduleId].logMask_[i] = logMask;
    }

    /* Allocate an internal memory log buffer for this facility
     * internalMemoryBufferSize is default (-1), meaning "don't touch" */
    if (internalMemoryBufferSize >= 0)
    {
        Logging_memory_destroy(&logItemList[moduleId].internalMemoryBuffer,
                logItemList[moduleId].internalMemorySize);
        logItemList[moduleId].internalMemorySize   = internalMemoryBufferSize;
        logItemList[moduleId].internalMemoryBuffer = Logging_memory_create(
                logItemList[moduleId].internalMemorySize);
    }

    return result;
}

Log::Mask_t Logging::getLogMask(const App_ModuleId_t moduleId) const
{
    assert(moduleId < LAST_ID);
    return(logItemList[moduleId].logMask_[0]);
}

const char* Logging::getLogName(const App_ModuleId_t moduleId) const
{
    assert(moduleId < LAST_ID);
    return logItemList[moduleId].name_;
}

Log::SequenceNumber Logging::getNewSequenceNumber(void)
{
    return Log::getNextSequenceNumber();
}

/** Logging::log()
 *      Format a log-message and log it to memory and then send it to syslog.
 * Input:
 *      logLevel    Log::Error, Log::Warning, Log::Debug, ...
 *      moduleId    APP_ID, PDP_ID, ...
 *      fileName    Auto generated by compiler
 *      lineNumber  Auto generated by compiler */
__attribute__ ((format(printf, 7, 0)))
void Logging::log(const Log::Level_t logLevel,
        const App_ModuleId_t moduleId,
        const char* const fileName,
        const int32_t lineNumber,
        const int facility,
        const char* const format,
        va_list argList)
{
    if (Logging_isLogEnabled(moduleId, logLevel))
    {
        char                       logMessage[LOG_MESSAGE_MAX_LENGTH];
        const size_t               logMessageSize       = sizeof(logMessage);
        size_t                     offset               = 0;
        struct RingBuffer_t* const internalMemoryBuffer = logItemList[moduleId].internalMemoryBuffer ?
                                                          logItemList[moduleId].internalMemoryBuffer : logBuffer;

        /* add sequence number to this log-message */
        {
            const uint16_t tmpLogSeqNo = getNewSequenceNumber();
            offset += Srv_Snprintf(logMessage + offset, logMessageSize - offset,
                    "%u ", tmpLogSeqNo);
        }

        logPrefixMutex.lockRead();
        if (logPrefix.length() > 0)
        {
            offset += Srv_Snprintf(logMessage + offset, logMessageSize - offset,
                    "%s ", logPrefix.c_str());
        }
        logPrefixMutex.unlock();

        // add module name, e.g. "PDP"
        offset += Srv_Snprintf(logMessage + offset, logMessageSize - offset,
                "%s", getLogName(moduleId));

        if (fileName && lineNumber)
        {
            offset += Srv_Snprintf(logMessage + offset, logMessageSize - offset,
                    " (%s, %i): ", fileName, (int)lineNumber);
        }
        else
        {
            offset += Srv_Snprintf(logMessage + offset, logMessageSize - offset,
                    ": ");
        }

        /* insert Log Level string for severity, skip NOTICE & INFO */
        if ((logLevel != Log::Notice) && (logLevel != Log::Info))
        {
            const char* str = Logging_getLogLevelString(logLevel);
            offset += Srv_Snprintf(logMessage + offset, logMessageSize - offset,
                    "%s: ", str);
        }

        offset += Srv_Vsnprintf(logMessage + offset, logMessageSize - offset,
                format, argList);

#if BACKTRACE_IN_SYSLOG == false
        size_t offsetBeforeBacktrace = offset;
#endif
        if ((logLevel <= Log::Warning) && (offset < logMessageSize))
        {
            /* if LogLevel is Log::Warning or more serious, then add a small
             * backtrace at the end of internalMemoryBuffer.
             * getBacktraceAsString(3, 8): 3 = do backtrace from App_Log()
             * macro level, i.e. skip:
             *   #x   ...
             *   #x+0   App_Log(...)
             *   #x+1     Logging_log()
             *   #x+2       app_doLog()
             * 6 mean take the next 6 frames and return as string to here. */
            const size_t      numOfFrames = (logLevel == Log::Emergency) ? 64 : 8;
            const std::string backtrace   = std::string(" BACKTRACE=") +
                                            srvlib::getBacktraceAsString(2, numOfFrames);
            Srv_Strncpy(logMessage + offset, backtrace.c_str(),
                    logMessageSize - offset);

            /* can't use backtrace.size() here,, because if logMessage +
             * backtrace did not fit into the buffer, then backtrace.size()
             * is larger than the number of added chars in Srv_Strncpy().
             * So, let's find out how many chars where added really. */
            const size_t backtraceSize = strlen(logMessage + offset);
            offset += backtraceSize;
        }

        /* If the log-message could not fit into our buffer,
         * then make the last chars "..." */
        if (offset >= logMessageSize)
        {
            /* String was truncated.
             * C99 returns length that would have been written.
             * Previous C standard returns -1.
             * Srv_Vsnprintf() wrapper never return < 0 (i.e. -1) */
            Srv_Strncpy(logMessage + logMessageSize - 5, " ...", 5);
            offset = logMessageSize - 1;
        }

        /* Store log in memory. This is useful when a core dump is created.
         * Then we have the last (say 128) logs for this instance and in
         * the core dump itself offset points at the null-termination of
         * the string,
         * '+1' = copy null_termination as well */
        Logging_memory_add(internalMemoryBuffer, moduleId, logMessage, offset + 1);

        /* Optionally chop off the backtrace before syslog */
#if BACKTRACE_IN_SYSLOG == false
        if (offset > offsetBeforeBacktrace)
        {
            offset             = offsetBeforeBacktrace;
            logMessage[offset] = '\0';
        }
#endif

        Log_doLog(logLevel, facility, identityString, logMessage);
    }
}

/* ------------------------------------------------------------------------- */
/* INTERFACE DATA                                                            */
/* ------------------------------------------------------------------------- */
static Logging logging;

/* ------------------------------------------------------------------------- */
/* INTERFACE FUNCTIONS                                                       */
/* ------------------------------------------------------------------------- */
void Logging_init(const std::string& str)
{
    logging.setIdentityString(str);
}

void Logging_setLogPrefix(const std::string& logPrefix)
{
    logging.setLogPrefix(logPrefix);
}

void Logging_setGetTimeofdayFn(Logging_GetTimeofdayFn_t fn)
{
    Logging_getTimeofdayFn = fn;
}

/** Logging_setLogMask()
 * Called from:
 *   - main()->App_Init()->App_SetModuleTable() - Application startup.
 *   - gtpb_GetOrSetModuleMask() - When debug-cli "app_setlogmask id level" is executed.
 *   - gtpb_OamConfigCommit() - When config is committed (i.e. when someone turn on/off e.g. debug-level).
 *   - UnitTests to turn off logging while testing */
Log::Mask_t Logging_setLogMask(const App_ModuleId_t moduleId, const Log::Mask_t logMask,
        const char* name, const int internalMemoryBufferSize)
{
    return logging.setLogMask(moduleId, logMask, name, internalMemoryBufferSize);
}

Log::Mask_t Logging_getLogMask(const App_ModuleId_t moduleId)
{
    return logging.getLogMask(moduleId);
}

/*bool Logging_isLogEnabled(const App_ModuleId_t moduleId, const Log::Level_t logLevel)
   {
    return logging.Logging_isLogEnabled(moduleId, logLevel);
   }*/

Log::Mask_t Logging_getLogMaskFromLevel(const Log::Level_t loglevel)
{
    Log::Mask_t logMask;

    if (loglevel >= (Log::LAST - 1))
    {
        logMask = Log::AllMask;
    }
    else
    {
        logMask = (Log::Mask_t) ((1 << (loglevel + 1)) - 1);
    }

    return logMask;
}

/** Logging_logInternal()
 *      Used when App_Log() level was not turned on and
 *      instead only log to memory. */
void Logging_logInternal(const App_ModuleId_t moduleId, const char* format, ...)
{
    va_list argList;
    va_start(argList, format);

    struct RingBuffer_t* const internalMemoryBuffer = logItemList[moduleId].internalMemoryBuffer ?
                                                      logItemList[moduleId].internalMemoryBuffer : logBuffer;

    Logging_memory_doLog(internalMemoryBuffer, moduleId, format, argList);

    va_end(argList);
}

void Logging_log(const Log::Level_t logLevel, const App_ModuleId_t moduleId,
        const char* format, ...)
{
    if (Logging_isLogEnabled(moduleId, logLevel))
    {
        va_list argList;
        va_start(argList, format);

        logging.log(logLevel, moduleId, NULL, 0,
                LOG_FACILITY_DEFAULT, format, argList);

        va_end(argList);
    }
}

void Logging_logExtended(const Log::Level_t logLevel,
        const App_ModuleId_t moduleId,
        const char* filename, const int32_t lineNumber,
        const char* format, ...)
{
    va_list argList;
    va_start(argList, format);

    logging.log(logLevel, moduleId, filename, lineNumber,
            LOG_FACILITY_DEFAULT, format, argList);

    va_end(argList);
}

void Logging_logToFacility(const Log::Level_t logLevel,
        const App_ModuleId_t moduleId, const int facility,
        const char* format, ...)
{
    va_list argList;
    va_start(argList, format);

    logging.log(logLevel, moduleId, NULL, 0,
            facility, format, argList);

    va_end(argList);
}

void Logging_logStrings(const Log::Level_t logLevel,
        const App_ModuleId_t moduleId, char* pBuf)
{
    const char* pSep   = "\n";
    char*       pToken = NULL;
    char*       pLast;

    pToken = strtok_r(pBuf, pSep, &pLast);
    while (pToken)
    {
        App_Log(logLevel, moduleId, "%s", pToken);
        pToken = strtok_r(NULL, pSep, &pLast);
    }
}

/** Logging_logSupressed()
 *  Helper function for App_LogMgr() to reduce the macro footprint */
void Logging_logSupressed(const App_ModuleId_t moduleId,
        const uint32_t nuberOfSupressedLogs)
{
    if (nuberOfSupressedLogs > 0)
    {
        App_Log(Log::Notice, moduleId, "Last message was suppressed %d times",
                nuberOfSupressedLogs);
    }
}

void App_Perror(const char* msg, App_ModuleId_t module)
{
    const char* s;
    static char buffer[100];
    int         myerrno;

    myerrno = errno;
    s       = strerror(myerrno);
    Srv_Snprintf(buffer, sizeof(buffer), "%s:%s(%d)", msg, s, myerrno);
    App_Log(Log::Error, module, "%s", buffer);
}
