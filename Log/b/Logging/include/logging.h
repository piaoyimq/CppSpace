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

#ifndef _LOGGING_H_
#define _LOGGING_H_

#include "app/include/module_id.h"
#include "ggsn.h"
#include "stdmacros.h"

#include <sstream>
#include <boost/static_assert.hpp>

#include <time.h>

/* ------------------------------------------------------------------------- */
struct RingBuffer_t;

/* ------------------------------------------------------------------------- */
/**
 * Help MACROS to suppress/restore log-messages in unit-tests
 *
 * LogMask to use:
 *      Log::NoneMask  - suppress all log-messages
 *      Log::DebugMask - suppress log-messages below log-level "DEBUG"
 *      Log::CriticalMask  - suppress log-messages below log-level "CRIT"
 *      Log::AllMask   - suppress no log-messages
 *
 * To suppress all log-messages except Log::Critical and above levels:
 *      UT_LOGLEVEL_SETPUSH(GSC, Log::CriticalMask);
 * To restore to previous level used before this unit-test was executed:
 *      UT_LOGLEVEL_POP(GSC);
 *
 * Use UT_LOGLEVEL_SET(module, logMask) to just set logLevel and
 * module name in logging framework for that module without
 * possibility to restore to previous logMask.
 *
 * Because the variable is with scope remember to do opposite order when
 * restoring many modules:
 *      UT_LOGLEVEL_PUSH(GSC, Log::Critical);
 *      UT_LOGLEVEL_PUSH(BOARDH, Log::Debug);
 *          :
 *      UT_LOGLEVEL_POP(BOARDH);
 *      UT_LOGLEVEL_POP(GSC);
 */

/*
 * UT_LOGMASK_ENABLE    we have the following to choose from:
 *      Log::AllMask    - Turn on ALL logs.
 *      Log::DebugMask  - Turn on DEBUG (but not DEBUG1-31) for moduleId.
 *
 * UT_LOGMASK_DISABLE   we have the following to choose from:
 *      Log::Emergency_MASK  - We should NEVER use EMERG unless it really is a
 *                        system wide fault.
 *      Log::CriticalMask   - This will silence all logging but enable when
 *                        crashing or other critical error.
 *      Log::NoneMask   - This will turn all off without any possibility to
 *                        even see EMERG errors (that should NEVER happen).
 */
#define UT_LOGMASK_ENABLE   Log::DebugMask
#define UT_LOGMASK_DISABLE  Log::CriticalMask

#define UT_LOGMASK_PUSH(moduleId, logMask)                                    \
    {                                                                         \
        const Log::Mask_t UT_logMask_ ## moduleId ## _old = Logging_setLogMask( \
                moduleId, logMask, #moduleId);

#define UT_LOGMASK_SET(moduleId, logMask)                                     \
    Logging_setLogMask(moduleId, logMask, #moduleId);

#define UT_LOGMASK_POP(moduleId)                                              \
    Logging_setLogMask(moduleId, UT_logMask_ ## moduleId ## _old);            \
    }

#define UT_SUPPRESS_LOG(moduleId, expr)                                       \
    do {                                                                      \
        const Log::Mask_t UT_logMask_ ## moduleId ## _old = Logging_setLogMask( \
                moduleId, UT_LOGMASK_DISABLE);                                \
        do { expr; } while (0); \
        Logging_setLogMask(moduleId, UT_logMask_ ## moduleId ## _old);        \
    } while (0) \

namespace Log
{

#define LOG_MESSAGE_MAX_LENGTH  1024

#define LOG_LOCAL5              (21 << 3)
#define LOG_LOCAL6              (22 << 3)

/**
 * LOG_LOCAL6 & LOG_LOCAL5 is defined in syslog.h so files using any of
 *      LOG_FACILITY_DEFAULT    Facility to be used by 'normal' logging.
 *      LOG_FACILITY_LI         Facility to be used for LI debugging.
 * When any of these are used we need to include syslog.h as well.
 * Note: This is not to be done in any more than framework files.
 */
#define LOG_FACILITY_DEFAULT    LOG_LOCAL6
#define LOG_FACILITY_LI         LOG_LOCAL5

/**
 *  Log level specified when the App_LogX functions are invoked.
 *
 *  Level Keyword  Level     Description                 Syslog Definition
 *  ----------------------------------------------------------------------
 *  emergencies      0       System unusable             Log::Emergency
 *  alerts           1       Immediate action needed     Log::Alert
 *  critical         2       Critical conditions         Log::Critical
 *  errors           3       Error conditions            Log::Error
 *  warnings         4       Warning conditions          Log::Warning
 *  notifications    5       Normal but significant      Log::Notice
 *                           condition
 *  informational    6       Informational messages      Log::Info
 *                           only
 *  debugging        7+      Debugging messages          Log::Debug
 *
 *  IMPORTANT:General guideline for level usage
 *  ----------------------------------------------
 *  Log::Emergency,Log::Alert,Log::Critical: internal failure such as a bug should be
 *  followed by a program abortion
 *  Log::Error: external errors such as a SGSN, or PPB misbehaving should
 *  discard service request
 *  Log::Warning: external exceptional condition such as SGSN restart
 *  Log::Notice: initialization phases
 *  Log::Info: normal internal events such as pdp creation, deletion
 */
enum Level_t
{
    Emergency =  0,
    Alert     =  1,
    Critical  =  2,
    Error     =  3,
    Warning   =  4,
    Notice    =  5,
    Info      =  6,
    Debug     =  7,
    Debug1    =  8,
    Debug2    =  9,
    Debug3    = 10,
    Debug4    = 11,
    Debug5    = 12,
    Debug6    = 13,
    Debug7    = 14,
    Debug8    = 15,
    Debug9    = 16,
    Debug10   = 17,
    Debug11   = 18,
    Debug12   = 19,
    Debug13   = 20,
    Debug14   = 21,
    Debug15   = 22,
    Debug16   = 23,
    Debug17   = 24,
    Debug18   = 25,
    Debug19   = 26,
    Debug20   = 27,
    Debug21   = 28,
    Debug22   = 29,
    Debug23   = 30,
    Debug24   = 31,

    /* This must be the last entry */
    LAST
};

BOOST_STATIC_ASSERT(Emergency   == 0);
BOOST_STATIC_ASSERT(Alert       == 1);
BOOST_STATIC_ASSERT(Critical    == 2);
BOOST_STATIC_ASSERT(Error       == 3);
BOOST_STATIC_ASSERT(Warning     == 4);
BOOST_STATIC_ASSERT(Notice      == 5);
BOOST_STATIC_ASSERT(Info        == 6);
BOOST_STATIC_ASSERT(Debug       == 7);
BOOST_STATIC_ASSERT(LOG_LOCAL5  == (21 << 3));
BOOST_STATIC_ASSERT(LOG_LOCAL6  == (22 << 3));

enum Mask_t
{
    NoneMask      = (0),
    DebugMask     = ((1 << Emergency) | (1 << Alert) | (1 << Critical) | (1 << Error) | (1 << Warning) | (1 << Notice) | (1 << Info) | (1 << Debug)),
    InfoMask      = ((1 << Emergency) | (1 << Alert) | (1 << Critical) | (1 << Error) | (1 << Warning) | (1 << Notice) | (1 << Info)),
    NoticeMask    = ((1 << Emergency) | (1 << Alert) | (1 << Critical) | (1 << Error) | (1 << Warning) | (1 << Notice)),
    CriticalMask  = ((1 << Emergency) | (1 << Alert) | (1 << Critical)),
    EmergencyMask = ((1 << Emergency)),
    AllMask       = (0xFFFFFFFF)
};

}  // namespace Log

/* strings for printing message level */
static __inline__ const char* Logging_getLogLevelString(const Log::Level_t logLevel)
{
    const char* const Log_LevelString[] =
    {
        "***EMERG",
        "***ALERT",
        "***CRIT",
        "***ERROR",
        "!!!WARN",
        "+++NOTICE",
        "INF",
        "DBG",
        "DBG1",     "DBG2",       "DBG3",       "DBG4",
        "DBG5",     "DBG6",       "DBG7",       "DBG8",
        "DBG9",     "DBG10",      "DBG11",      "DBG12",
        "DBG13",    "DBG14",      "DBG15",      "DBG16",
        "DBG17",    "DBG18",      "DBG19",      "DBG20",
        "DBG21",    "DBG22",      "DBG23",      "DBG24"
    };
    return (logLevel < (Log::Level_t)Array_Size(Log_LevelString)) ?
           Log_LevelString[logLevel] : "*ERROR IN MESSAGE LEVEL*";
}

/*
 * Prototype for Logging to query current time (for logging etc).
 */
typedef timeval (* Logging_GetTimeofdayFn_t) ();
extern Logging_GetTimeofdayFn_t Logging_getTimeofdayFn;

/******************************************************************************
   Function:    Logging_init
   Description: Set identity string, used for syslog and to uniquely
                identify the process in log message on this board.
                Call once before logging.
******************************************************************************/
void Logging_init(const std::string& identityString);

/******************************************************************************
   Function:    Logging_setLogPrefix
   Description: Set optional log prefix. If set, this is inserted
                between sequence number and module/component name.
                Can be called many times during the life time of a process.

******************************************************************************/
void Logging_setLogPrefix(const std::string& logPrefix);

/******************************************************************************
   Function:    Logging_SetTimeOfDayFn
   Description: Set function to use to read time of day.
   Return:      None.
   Output:      None.
   Modified:    None.
   Input:       fn, function to register in Logging fraework.
******************************************************************************/
void Logging_setGetTimeofdayFn(Logging_GetTimeofdayFn_t fn);

/******************************************************************************
   Function:    Logging_getLogMask
   Description: Returns the log mask for a module.
   Return:      log mask
   Output:      None.
   Modified:    None.
   Input:       ModuleId: Module Id to get the mask.
******************************************************************************/
Log::Mask_t Logging_getLogMask(const App_ModuleId_t moduleId);

/******************************************************************************
   Function:    Logging_getLogMaskFromLevel
   Description: Returns log mask for logLevel and all lower levels set to '1'
   Return:      log mask for multiple levels.
   Output:      None.
   Modified:    None.
   Input:       logLevel: Log level.
******************************************************************************/
Log::Mask_t Logging_getLogMaskFromLevel(const Log::Level_t loglevel);

/******************************************************************************
   Function:    Logging_setLogMask
   Description: Sets the log mask for a module identified by ModuleId
   Return:      Previous log mask for moduleId
   Output:      None.
   Modified:    None.
   Input:       ModuleId: The module identification.
                LogMask: New log mask.
******************************************************************************/
Log::Mask_t Logging_setLogMask(const App_ModuleId_t moduleId,
        const Log::Mask_t logMask,
        const char* name = NULL,
        const int internalMemoryBufferSize = -1);

/******************************************************************************
   Function:    Logging_isLogEnabled
   Description: This function should be used in in a App_Log wrapper function
                in order to verify if logging activities have to be performed
                for a given module.
   Return:      Returns true (1) if the module will produce log for the given
                level. false (0) otherwise.
   Output:      None.
   Modified:    None.
   Input:       ModuleId: Id of the module that requires logging.
                LogLevel: Log level that is being tested.
******************************************************************************/
#define MODULE_NAME_MAX_SIZE 10
struct LogItem_t
{
    char name_[MODULE_NAME_MAX_SIZE]; // e.g. "PDP\0"

    /* logMask_ - LogLevel for a Module_Id (e.g. GSC_ID)
     * All Modules are initialized at startup from gtpcd.cc
     * To set a new log-level for one module:
     *      Logging_setLogMask(moduleId, logMask)
     * When all 32 bits are set, logging is done at all levels for this module.
     * When setting a new log-level like this, current log-level is
     * pushed to the stack.
     * push a new log-level for Module_Id:
     *      Logging_pushLogMask(moduleId, logMask);
     * current logMask for moduleId will be pushed down in the stack, and the
     * one on top of the stack will be lost.
     * Later, same level can be pop'ed back like this:
     *      Logging_popLogMask(moduleId);
     * Why use push/pop?
     * This is to prepare for when we want to temporary raise log-level
     * for a signaling flow, e.g. we receive a signal from GTP for IMSI-X
     * This IMSI is tagged to be logged with DEBUG level so we temporary
     * push DEBUG level to all levels (or some?). Then later when all
     * is done for that message and we get back to "Core_Loop" again, we
     * can pop back to "old" log-level.
     * To push to all modules at once, do:
     *      Logging_pushLogLevel(LAST_ID, log-level);
     * To pop back all at the same time:
     *      Logging_popLogLevel(LAST_ID);
     *
     * Code support any stack-size.
     *      logMask_[0] = log-level that is in use right now.
     *      logMask_[1] = log-level that will be used after a pop
     *                    (e.g. was pushed recently)
     * What happen if we pop a new log-mask without doing a push?
     *      Don't do that!
     *      However, should someone do that by mistake, all stack'ed levels
     *      are initialized to the same log-level the first time when set
     *      (or when set with a name the first time).
     * Note:
     *      Array-size 1 is also supported meaning that it is not in use.
     *      The compiler will optimize code so that no extra cpu-time is
     *      wasted due to this. It is not in use today!
     * If not in use, why not remove it?
     *      It could be put in use in later version of code.
     * Complications?
     *      It will not work for multiple threaded applications.
     *      It would work if we only do logging in Main-thread in
     *      PSC-instances, on PCS-main we have many threads reading IPC I/O.
     *      If we would constrain push/pop so that they only run on
     *      PSC-instances, then it could be used for example to raise log-level
     *      for a certain Ue(imsi).
     * TODO:
     *      Remove this code if we don't start using it.
     */
    Log::Mask_t logMask_[1];

    /* When logging to internal memory this pointer hold this
     * modules own buffer for this. */
    size_t               internalMemorySize;
    struct RingBuffer_t* internalMemoryBuffer;
};

extern struct LogItem_t logItemList[LAST_ID];
static __inline__ bool Logging_isLogEnabled(const App_ModuleId_t moduleId,
        const Log::Level_t logLevel)
{
    const Log::Mask_t logMask = logItemList[moduleId].logMask_[0];
    return ((logMask >> logLevel) & 1);
}

/* used to add log-messages to internal-log-buffer for log-levels that are
 * not on (e.g. Log::Info) */
void Logging_logInternal(const App_ModuleId_t moduleId,
        const char* format, ...) __attribute__ ((format(printf, 2, 3)));

/* used by App_Log() macro to log messages to sys-log (all messages logged
 * are logged to internal-log-buffer as well) */
void Logging_log(const Log::Level_t logLevel, const App_ModuleId_t moduleId,
        const char* format, ...) __attribute__ ((format(printf, 3, 4)));

inline
void Logging_log(const Log::Level_t logLevel, const App_ModuleId_t moduleId)
{
    Logging_log(logLevel, moduleId, "%s", "");
}

#if defined(_MSC_VER)

#define App_Log          Logging_log
#define App_LogExtended  Logging_logExtended
#define App_LogLttng     App_LogLttng

#else /* !_MSC_VER */

#if defined(SEOS_TARGET) && (defined (CONFIG_LTTNG_SSR) || defined (CONFIG_LTTNG_SSR_SIM))

#include "app/Logging/include/app_lttng_debuglog.h"

/* Strange naming, but this follows the nomenclature of Logging_log above */
extern "C" void Logging_logLttng(Log::Level_t loglevel, const char* moduleId,
        const char* format, ...) __attribute__ ((format(printf, 3, 4)));

#define App_LogLttng(logLevel, moduleId, format, ...)                         \
    do {                                                                      \
        STAP_PROBEV(tracepoint_ericsson_epg_pgw_applog,                       \
                debuglog, ## __VA_ARGS__);                                    \
        if (likely(__tracepoint_ericsson_epg_pgw_applog___debuglog.state))    \
            Logging_logLttng(logLevel, App_GetModuleName(moduleId),           \
                    format, ## __VA_ARGS__);                                  \
    } while (0)

#else
#define App_LogLttng(...)
#endif

/******************************************************************************
   Function:     App_Log
   Description: prepend automatically CPB specific info to system log message
                (syslog wrapper).

                Can be extended to take special actions when the level is in
                the 3 highest levels (emergency, alert, critical) for example
                exit or abort.
   Return:      None
   Output:      None
   Modified:    None
   Input:       Log::Level_t:  one of Log::Emergency, Log::Alert, etc.

   Level Keyword  Level     Description                 Syslog Definition
   ----------------------------------------------------------------------
   emergencies      0       System unusable             Log::Emergency
   alerts           1       Immediate action needed     Log::Alert
   critical         2       Critical conditions         Log::Critical
   errors           3       Error conditions            Log::Error
   warnings         4       Warning conditions          Log::Warning
   notifications    5       Normal but significant      Log::Notice
                            condition
   informational    6       Informational messages      Log::Info
                            only
   debugging        7       Debugging messages          Log::Debug

   IMPORTANT: General guideline for level usage
   ----------------------------------------------
   Log::Emergency, Log::Alert, Log::Critical:
       internal failure such as a bug should be followed by a program abortion
   Log::Error:
       external errors such as a SGSN, or PPB misbehaving should discard
       service request
   Log::Warning:
       external exceptional condition such as SGSN restart
   Log::Notice:
       initialization phases
   Log::Info:
       normal internal events such as pdp creation, deletion

   NOTE:
 * Check log level before calling any logging function.
 * This is a big performance win for applications that log a lot.
 * Add check for LTTNG to add INFO level as default
 *
 *****************************************************************************/
#define App_Log LOG
#define LOG(logLevel, moduleId, ...)                                          \
    do {                                                                      \
        if ((unlikely(Logging_isLogEnabled(moduleId, logLevel))))             \
        {                                                                     \
            Logging_log(logLevel, moduleId, __VA_ARGS__);                     \
            App_LogLttng(logLevel, moduleId, __VA_ARGS__);                    \
        }                                                                     \
    } while (0)

#define App_Log2(logLevel, moduleId, output)                                  \
    do {                                                                      \
        if ((unlikely(Logging_isLogEnabled(moduleId, logLevel))))             \
        {                                                                     \
            std::stringstream ss;                                             \
            ss << output;                                                     \
            App_Log(logLevel, moduleId, "%s", ss.str().c_str());              \
        }                                                                     \
    } while (0)

/** TRACE(...)
 *      Use this App_Log() similar macro to store in internal-log-memory as well
 *      When using this, simply replace the App_Log() macro to TRACE() and
 *      if logging is enabled for moduleId and logLevel, then it works
 *      as App_Log. If logging was turned off, the log goes into internal
 *      memory log only.
 */
#define TRACE(logLevel, moduleId, ...)                                        \
    do {                                                                      \
        if ((unlikely(Logging_isLogEnabled(moduleId, logLevel))))             \
        {                                                                     \
            Logging_log(logLevel, moduleId, __VA_ARGS__);                     \
        }                                                                     \
        else                                                                  \
        {                                                                     \
            Logging_logInternal(moduleId, __VA_ARGS__);                       \
        }                                                                     \
        App_LogLttng(logLevel, moduleId, __VA_ARGS__);                        \
    } while (0)

#define App_LogExtended(logLevel, modidfilenameline, ...)                     \
    do {                                                                      \
        if (unlikely((Logging_getLogMask(modidfilenameline) >> (logLevel)) & 1)) {  \
            Logging_logExtended(logLevel, modidfilenameline, __VA_ARGS__);    \
        }                                                                     \
    } while (0)

#endif /* !_MSC_VER */

/******************************************************************************
   Function:    App_LogExtended
   Description: Same as App_Log but with the possibility to add a file name
                and line number. Note that in order to have the file name and
                the line number logged _both_ have to be non-null.
   Return:      None.
   Output:      None.
   Modified:    None.
   Input:       None.
******************************************************************************/
void Logging_logExtended(const Log::Level_t logLevel,
        const App_ModuleId_t moduleId,
        const char* filename,
        const int32_t lineNumber,
        const char* format, ...)
__attribute__ ((format(printf, 5, 6)));

/******************************************************************************
   Function:    Logging_logToFacility
   Description: Same as App_Log but with the possibility to send log to a
                different facility. Facilities to be used are define in this
                file. You can configure logging in the /etc/syslog.conf.
   Return:      None.
   Output:      None.
   Modified:    None.
   Input:       None.
******************************************************************************/
void Logging_logToFacility(const Log::Level_t logLevel,
        const App_ModuleId_t moduleId,
        const int facility,
        const char* format, ...)
__attribute__ ((format(printf, 4, 5)));

/******************************************************************************
   Function:    Logging_logStrings
   Description: Accepts a string buffer and prints each line (separated by \n)
                separately using App_Log.
   Return:      None.
   Output:      None.
   Modified:    pBuf : The string to print (will be tokenized).
   Input:       logLevel : Log level.
                moduleId : Module id.
******************************************************************************/
void Logging_logStrings(const Log::Level_t logLevel,
        const App_ModuleId_t moduleId,
        char* pBuf);

/* ------------------------------------------------------------------------- */
/* Helper function for App_LogMgr() to reduce the macro footprint. */
void Logging_logSupressed(const App_ModuleId_t ModuleId, const uint32_t n);

/*
 * Simple log manager that suppresses all logs but 1 every 2 seconds.
 * Simpler to use and more efficient than the other log manager where you
 * explicitly have to create error classes for each error.
 *
 * Example:
 * App_LogMgr(Log::Error, PDP_ID, "%s: This is a serious error!", __FUNCTION__);
 */
#define App_LogMgr(logLevel, moduleId, ...)                                   \
    do {                                                                      \
        static uint32_t _n = 0;                                               \
        static time_t   _t = 0;                                               \
        if (unlikely(Logging_isLogEnabled(moduleId, logLevel))) {             \
            const time_t _now = Logging_getTimeofdayFn().tv_sec;              \
            if (_now - _t > 2) {                                              \
                Logging_log(logLevel, moduleId, __VA_ARGS__);                 \
                Logging_logSupressed(moduleId, _n);                           \
                _t = _now;                                                    \
                _n = 0;                                                       \
            } else {                                                          \
                _n++;                                                         \
            }                                                                 \
        }                                                                     \
    } while (0)

/* ------------------------------------------------------------------------- */
// Logs a message and includes errno
void App_Perror(const char* msg, App_ModuleId_t module);

/* ------------------------------------------------------------------------- */
#endif /* _LOGGING_H_ */
