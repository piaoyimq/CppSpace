#ifndef SRVLIB_INCLUDE_LOG_MNG_H_
#define SRVLIB_INCLUDE_LOG_MNG_H_
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

#include "app/include/app.h"
#include <stdarg.h>
#include <stdint.h>
#include <sys/types.h>

/******************************************************
 * LOG MANAGER DEFINITIONS
 *******************************************************/

#define LOG_MNG_MAX_ERRORS     50 /* Max number of errors that can be logged */
#define LOG_MNG_SCRATCH_PERIOD 2  /* time in seconds after which history is scratched */
#define LOG_MNG_SILENT_LOG     50 /* number of errors to log silently */
#define LOG_MNG_SILENT_LOG_TIME 0 /* default time to log silently */

#define LOG_MNG_STATS_OFF      0
#define LOG_MNG_STATS_ON       1

#define LOG_MNG_ADVANCED_OFF   0
#define LOG_MNG_ADVANCED_ON    1

#define LOG_MNG_ADVANCED_DATA_SAVE               0 /* always save data */
#define LOG_MNG_ADVANCED_DATA_SAVE_FIRST         1 /* save data only first time */
#define LOG_MNG_ADVANCED_DATA_SAVE_NEVER         2 /* never save data */

#define LOG_MNG_ADVANCED_DATA_MAX          1000000  /* Maximum byte allowed for advanced data */

#define LOG_MNG_DIAGNOSTIC_RUN_ON_DISPLAY  0  /* Run each time the error is diplayed  */
#define LOG_MNG_DIAGNOSTIC_RUN_EACH_TIME   1  /* Run each time the error occurs       */
#define LOG_MNG_DIAGNOSTIC_RUN_ONCE        2  /* Run only the first time (on display) */


#define LOG_MNG_DATA_BLOCK_SIZE    10000  /* Data block size (bytes) for advanced manager buffer */

#define LOG_MNG_MAX_LOG_STRING 1000

/* Just to make life somewhat easier... */
#define LOG_MGR_CFG(logMgr, eId, eStr, st, t, n) do { \
        log_mng_NewError(logMgr,  eId, eStr); \
        log_mng_SetOption(logMgr, eId, LOG_MNG_OPT_USE_STATS,       st);  /* Use stats */ \
        log_mng_SetOption(logMgr, eId, LOG_MNG_OPT_SILENT_LOG_TIME, t);   /* Number of logs per second */ \
        log_mng_SetOption(logMgr, eId, LOG_MNG_OPT_SHOW_FIRST_N,    n);   /* Number of occurrences to show before hiding any */ \
} while (0)

/******************************************************
 * LOG MANAGER TYPES
 *******************************************************/


struct log_mng_advanced_iter_t
{
    char* pIt;
    int   Size;
    int   MaxSize;
};


typedef int (* log_mng_logCb)(char* logString, const void* pArg, const char* format, va_list argList);

typedef int (* log_mng_HandleDataCb)(log_mng_advanced_iter_t* pIt, int NofElem, char* pOutBuf, int Size);
typedef int (* log_mng_Diagnostics)(va_list ArgList);

enum log_mng_error_opt_e
{
    LOG_MNG_OPT_SCRATCH_PERIOD,
    LOG_MNG_OPT_SILENT_LOG,
    LOG_MNG_OPT_SILENT_LOG_TIME, /* 1 / no of logs per second */
    LOG_MNG_OPT_SHOW_FIRST_N,
    LOG_MNG_OPT_USE_STATS,
    LOG_MNG_OPT_ADVANCED,
    LOG_MNG_OPT_ADVANCED_DATA,
    LOG_MNG_OPT_ADVANCED_DATA_MAX,
    LOG_MNG_OPT_DIAGNOSTIC
};

/* used to manually configure the log manager */

struct log_mng_error_opt_t
{
    unsigned int ScratchPeriod;     /* Seconds, time to keep error history */
    unsigned int SilentLog;         /* Number of occurrences to hide */
    unsigned int SilentLogTime;     /* Number of seconds to hide. Should be better granularity?? */
    int          ShowFirstN;        /* Nuber of occurrences to show before hiding any */
    unsigned int UseStats;          /* Use Stats feature */
    unsigned int Advanced;          /* Use Advanced feature */
    unsigned int AdvancedData;      /* advancd data option */
    int          AdvancedDataMax;   /* Max memory in bytes allowed for advanced data */
    unsigned int Diagnostic;        /* Use diagnostics feature */
};


struct log_mng_advanced_t
{
    char*                pData;
    char*                pRead;
    char*                pWrite;
    int                  SizeOfBuffer;
    int                  SizeOfData;
    int                  NofElements;
    log_mng_HandleDataCb fHandleData;
};


struct log_mng_error_stats_t
{
    time_t   First;         /* Timestamp (UTC) for first occurrence */
    time_t   Last;          /* Timestamp (UTC) for last occurrence */
    uint64_t TotalNofError;
    int      NofScratch;    /* Updates each time history is scratched */
};

struct log_mng_error_t
{
    int                    ErrorId;     /* This is a real ID that identifies the error */
    char                   Error[64];   /* Name of the error */
    int                    ErrorSet;    /* indicates if the Error attribute is set */
    int                    NofRep;      /* Counts the times the error was logged  */
    int                    nThreshold;  /* Repeat threshold until the error is printed */
    time_t                 LastUpdate;  /* timestamp for last update of error, UTC */
    time_t                 LastPrinted; /* timestamp for last printout of error, UTC */
    log_mng_error_stats_t* Stats;       /* Statistical data for the error */
    log_mng_advanced_t*    pAdvancedMng;
    log_mng_Diagnostics    fDiagnostics;
    log_mng_error_opt_t    Options;         /* Configuration options */
};


struct log_mng_t
{
    int             Id;   /* Log manager id is the name of the module in which it is initiated */
    log_mng_error_t Errors[LOG_MNG_MAX_ERRORS];
    int             NofErrors; /* The number of initiated errors in the log manager */
};


enum log_mng_result_e
{
    LOG_MNG_SUCCESS,
    LOG_MNG_OPERATION_FAILED,
    LOG_MNG_ERROR_NOT_FOUND,
    LOG_MNG_ERROR_ALREADY_EXISTS,
    LOG_MNG_IS_NULL,
    LOG_MNG_ERROR_REGISTER_EXHAUSTED,
    LOG_MNG_ADV_DATA_READ_ERROR
};



/******************************************************
 * LOG MANAGER CONS/DIST FUNCTIONS
 *******************************************************/

log_mng_t* log_mng_NewLogMng();
void log_mng_DestroyLogMng(log_mng_t* pLogMng);


/******************************************************
 * LOG MANAGER OPTIONS FUNCTIONS
 *******************************************************/

void log_mng_InitOptions(log_mng_error_opt_t* pOpt);

log_mng_result_e log_mng_SetOption(log_mng_t* pLogMng, int ErrId,
        log_mng_error_opt_e Option, int Value);

log_mng_result_e log_mng_SetOptions(log_mng_t* pLogMng, int ErrorId,
        log_mng_error_opt_t* pOpt);

log_mng_result_e log_mng_GetOptions(log_mng_t* pLogMng, int ErrId, log_mng_error_opt_t* pOpt);



/******************************************************
 * LOG MANAGER FUNCTIONS
 *******************************************************/

log_mng_result_e log_mng_NewError(log_mng_t* pLogMng, int ErrorId, const char* pFormat);
log_mng_result_e log_mng_SetId(log_mng_t* pLogMng, int Id);


/******************************************************
 * LOG MANAGER STATS FUNCTIONS
 *******************************************************/
int log_mng_StatsSnprintf(log_mng_t* pLogMng, char* pBuf, int Size);

/******************************************************
 * LOG MANAGER DIAGNOSTIC FUNCTIONS
 *******************************************************/

log_mng_result_e log_mng_SetDiagnostics(log_mng_t* pLogMng, int ErrId, log_mng_Diagnostics fFun);

void log_mng_GetArgInt(va_list* ArgList, int* pD);
void log_mng_GetArgChar(va_list* ArgList, char* pC);
void log_mng_GetArgString(va_list* ArgList, char** pS);
void log_mng_GetArgUint(va_list* ArgList, unsigned int* pD);
void log_mng_GetArgUlong(va_list* ArgList, unsigned long* pD);


/******************************************************
 * LOG MANAGER MAIN FUNCTION
 *******************************************************/
log_mng_result_e log_mng_log(log_mng_t* pLogMng, Log::Level_t Level, App_ModuleId_t ModuleId,
        int ErrorId,  const char* Format, ...) __attribute__ ((format(printf, 5, 6)));

log_mng_result_e log_mng_log2(log_mng_t* pLogMng, Log::Level_t Level, App_ModuleId_t ModuleId,
        int ErrorId, log_mng_logCb logCb, const void* pArg, const char* Format, va_list ArgList) __attribute__ ((format(printf, 7, 0)));

log_mng_result_e log_mng_log3(log_mng_t* pLogMng, Log::Level_t Level, App_ModuleId_t ModuleId,
        int ErrorId, log_mng_logCb logCb, void* pArg);

#if defined(_MSC_VER)
#define LOG_MNG log_mng_log
#else
#define LOG_MNG(pLogMng, Level, ModuleId, ErrorId, format ...)            \
    do {                                                                   \
        if (Logging_isLogEnabled(ModuleId, Level)) {                             \
            log_mng_log((pLogMng), (Level), (ModuleId), (ErrorId), format);    \
        }                                                                    \
    } while (0)
#endif

#if defined(_MSC_VER)
#define LOG_MNG2 log_mng_log2
#else
#define LOG_MNG2(pLogMng, Level, ModuleId, ErrorId, LogCb, pArg, format ...)            \
    do {                                                                   \
        if (Logging_isLogEnabled(ModuleId, Level)) {                             \
            log_mng_log2((pLogMng), (Level), (ModuleId), (ErrorId), (LogCb), (pArg), format);    \
        }                                                                    \
    } while (0)
#endif

#if defined(_MSC_VER)
#define LOG_MNG3 log_mng_log3
#else
#define LOG_MNG3(pLogMng, Level, ModuleId, ErrorId, LogCb, pArg)            \
    do {                                                                   \
        if (Logging_isLogEnabled(ModuleId, Level)) {                             \
            log_mng_log3((pLogMng), (Level), (ModuleId), (ErrorId), (LogCb), (pArg));    \
        }                                                                    \
    } while (0)
#endif

/******************************************************
 * LOG MANAGER ADVANCED FUNCTIONS
 *******************************************************/

log_mng_result_e log_mng_SetAdvUserFunction(log_mng_t* pLogMng, int ErrId, log_mng_HandleDataCb fFun);

log_mng_result_e log_mng_DeleteData(log_mng_t* pLogMng, int ErrId);

int log_mng_AdvGetString(log_mng_advanced_iter_t* pIt, char* pStr);
int log_mng_AdvGetInt(log_mng_advanced_iter_t* pIt, int* pDout);
int log_mng_AdvGetChar(log_mng_advanced_iter_t* pIt, char* pCout);
int log_mng_AdvGetUInt(log_mng_advanced_iter_t* pIt, unsigned int* pUout);
int log_mng_AdvGetULong(log_mng_advanced_iter_t* pIt, unsigned long* pLout);

log_mng_result_e log_mng_AdvInitDataIter(log_mng_t* pLogMng, int ErrId,
        log_mng_advanced_iter_t* pIt);

#endif /* SRVLIB_INCLUDE_LOG_MNG_H_ */
