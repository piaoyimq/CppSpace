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

#include "app/Logging/include/log_mng.h"
#include "app/include/app.h"
#include "app/include/app_global.h"
#include "srvlib/include/srvutil.h"
#include "monotonic_time/monotonic_time.h"

#include <stdio.h>
#include <time.h>
#include <string.h>

static int log_mng_SaveData(va_list ap, const char* fmt, char* buf);
static log_mng_error_t* log_mng_GetError(log_mng_t* pLogMng, int ErrorId, int* Index);

static log_mng_advanced_t* log_mng_NewAdvancedMng();
static int log_mng_ExtendAdvancedDataBuffer(log_mng_advanced_t* pAdvMng);
static void log_mng_DestroyStats(log_mng_error_stats_t* pStats);

/******************************************************
* LOG MANAGER CONS/DIST FUNCTIONS
******************************************************/

log_mng_t* log_mng_NewLogMng()
{
    return new log_mng_t();
}

void log_mng_DestroyLogMng(log_mng_t* pLogMng)
{
    log_mng_error_t* pErr = NULL;
    int              i;

    if (pLogMng)
    {
        /* Free stats is needed */
        for (i = 0; i < LOG_MNG_MAX_ERRORS; i++)
        {
            pErr = &pLogMng->Errors[i];

            if (pErr->Stats)
                log_mng_DestroyStats(pErr->Stats);

            delete pErr->pAdvancedMng;
            pErr->pAdvancedMng = NULL;
        }
    }

    /* Free log manager */
    delete pLogMng;
}

/******************************************************
* LOG MANAGER STATS FUNCTIONS
******************************************************/

static log_mng_error_stats_t* log_mng_NewStats()
{
    log_mng_error_stats_t* pStats = new log_mng_error_stats_t();
    return pStats;
}

static void log_mng_DestroyStats(log_mng_error_stats_t* pStats)
{
    delete pStats;
}

static void log_mng_StatsUpdateFirst(log_mng_error_t* pErr, time_t Timestamp)
{
    if (pErr->Stats)
        pErr->Stats->First = Timestamp;
}

static void log_mng_StatsUpdateLast(log_mng_error_t* pErr, time_t Timestamp)
{
    if (pErr->Stats)
        pErr->Stats->Last = Timestamp;
}

static void log_mng_StatsUpdateScratch(log_mng_error_t* pErr)
{
    if (pErr->Stats)
        pErr->Stats->NofScratch++;
}

static void log_mng_StatsUpdateTotal(log_mng_error_t* pErr)
{
    if (pErr->Stats)
        pErr->Stats->TotalNofError++;
}



int log_mng_StatsSnprintf(log_mng_t* pLogMng, char* pBuf, int Size)
{
    int                    i;
    log_mng_error_t*       pErr    = NULL;
    log_mng_error_stats_t* pStats  = NULL;
    int                    NofChar = 0;

    App_Log(Log::Debug, SRVLIB_ID, "log_mng_StatsSnprintf");

    for (i = 0; i < pLogMng->NofErrors; i++)
    {
        if (pLogMng->Errors[i].Options.UseStats && pLogMng->Errors[i].Stats &&
            (pLogMng->Errors[i].Stats->NofScratch > 0))
        {

            NofChar += Srv_Snprintf(pBuf + NofChar, Size - NofChar, "*********** ERROR STATISTICS **************\n");
            break;
        }
    }

    for (i = 0; i < pLogMng->NofErrors; i++)
    {
        pErr   = &pLogMng->Errors[i];
        pStats = pErr->Stats;

        if (pErr->Options.UseStats && pErr->Stats && (pErr->Stats->TotalNofError > 0))
        {
            time_t     tFirst, tLast;
            struct tm  resultTimeFirst, resultTimeLast;
            struct tm* tmFirst, tmFirstStat;
            struct tm* tmLast, tmLastStat;

            tFirst  = pStats->First;
            tLast   = pStats->Last;

            tmFirst = localtime_r(&tFirst, &resultTimeFirst);
            memcpy(&tmFirstStat, tmFirst, sizeof(struct tm));
            tmLast  = localtime_r(&tLast, &resultTimeLast);
            memcpy(&tmLastStat, tmLast, sizeof(struct tm));

            NofChar += Srv_Snprintf(pBuf + NofChar, Size - NofChar,
                    "Error id          %d\n"
                    "Name              %s\n"
                    "First occurense     %02d:%02d:%02d\n"
                    "Last occurense      %02d:%02d:%02d\n"
                    "Scratched           %d\n"
                    "Total occurrences   %llu\n",
                    pErr->ErrorId, pErr->Error, tmFirstStat.tm_hour, tmFirstStat.tm_min, tmFirstStat.tm_sec,
                    tmLastStat.tm_hour, tmLastStat.tm_min, tmLastStat.tm_sec,
                    pStats->NofScratch, (long long unsigned int)pStats->TotalNofError);

            /* If the advanced function is defined, call the user function */
            if (pErr->Options.Advanced && pErr->pAdvancedMng)
            {
                if (pErr->pAdvancedMng->fHandleData)
                {
                    log_mng_advanced_iter_t It;
                    log_mng_AdvInitDataIter(pLogMng, pErr->ErrorId, &It);
                    NofChar += (*pErr->pAdvancedMng->fHandleData)(&It, pErr->pAdvancedMng->NofElements, pBuf + NofChar,
                            Size - NofChar);
                }
            }
        }
    }

    return NofChar;
}


int log_mng_StatsSizeof(log_mng_t* pLogMng)
{
    char buf[1000];
    return log_mng_StatsSnprintf(pLogMng, buf, sizeof(buf));
}



/******************************************************
* LOG MANAGER OPTIONS FUNCTIONS
******************************************************/

void log_mng_InitOptions(log_mng_error_opt_t* pOpt)
{
    pOpt->ScratchPeriod   = LOG_MNG_SCRATCH_PERIOD;
    pOpt->SilentLog       = LOG_MNG_SILENT_LOG;
    pOpt->SilentLogTime   = LOG_MNG_SILENT_LOG_TIME;
    pOpt->UseStats        = LOG_MNG_STATS_OFF;
    pOpt->Advanced        = LOG_MNG_ADVANCED_OFF;
    pOpt->AdvancedData    = LOG_MNG_ADVANCED_DATA_SAVE;
    pOpt->AdvancedDataMax = LOG_MNG_ADVANCED_DATA_MAX;
}

log_mng_result_e log_mng_SetOption(log_mng_t* pLogMng, int ErrId, log_mng_error_opt_e Option, int Value)
{
    log_mng_result_e Res = LOG_MNG_SUCCESS;

    if (pLogMng)
    {
        int              Index;
        log_mng_error_t* pErr = log_mng_GetError(pLogMng, ErrId, &Index);
        if (pErr)
        {
            switch (Option)
            {
                case LOG_MNG_OPT_SCRATCH_PERIOD:
                    pErr->Options.ScratchPeriod = Value;
                    break;
                case LOG_MNG_OPT_SILENT_LOG:
                    pErr->Options.SilentLog = Value;
                    break;
                case LOG_MNG_OPT_SILENT_LOG_TIME:
                    pErr->Options.SilentLogTime = Value;
                    break;
                case LOG_MNG_OPT_SHOW_FIRST_N:
                    pErr->Options.ShowFirstN = Value;
                    break;
                case LOG_MNG_OPT_USE_STATS:
                    pErr->Options.UseStats = Value;
                    if (pErr->Options.UseStats && !pErr->Stats)
                        pErr->Stats = log_mng_NewStats();
                    break;
                case LOG_MNG_OPT_ADVANCED:
                    pErr->Options.Advanced = Value;
                    if (pErr->Options.Advanced && !pErr->pAdvancedMng)
                        pErr->pAdvancedMng = log_mng_NewAdvancedMng();
                    break;
                case LOG_MNG_OPT_ADVANCED_DATA:
                    pErr->Options.AdvancedData = Value;
                    break;
                case LOG_MNG_OPT_ADVANCED_DATA_MAX:
                    pErr->Options.AdvancedDataMax = Value;
                    break;
                case LOG_MNG_OPT_DIAGNOSTIC:
                    pErr->Options.Diagnostic = Value;
                    break;

                default:
                    App_Log(Log::Debug, SRVLIB_ID, "Unsuported Options");
            }
        }
    }
    else
    {
        Res = LOG_MNG_ERROR_NOT_FOUND;
    }
    return Res;
}

log_mng_result_e log_mng_SetOptions(log_mng_t* pLogMng, int ErrorId, log_mng_error_opt_t* pOpt)
{
    log_mng_result_e Res = LOG_MNG_SUCCESS;

    if (pLogMng)
    {
        int              ErrorIndex;
        log_mng_error_t* pErr = log_mng_GetError(pLogMng, ErrorId, &ErrorIndex);
        if (pErr)
        {
            App_Log(Log::Debug, SRVLIB_ID, "log_mng_SetOptions: got error %d, advanced %d, Options advanced %d\n",
                    pErr->ErrorId, pErr->Options.Advanced, pOpt->Advanced);

            pErr->Options.ScratchPeriod   = pOpt->ScratchPeriod;
            pErr->Options.SilentLog       = pOpt->SilentLog;
            pErr->Options.SilentLogTime   = pOpt->SilentLogTime;
            pErr->Options.ShowFirstN      = pOpt->ShowFirstN;
            pErr->Options.UseStats        = pOpt->UseStats;
            pErr->Options.Advanced        = pOpt->Advanced;
            pErr->Options.AdvancedData    = pOpt->AdvancedData;
            pErr->Options.AdvancedDataMax = pOpt->AdvancedDataMax;

            /* If UseStats is specified and a Stats object has not been created previously */
            if (pErr->Options.UseStats && !pErr->Stats)
                pErr->Stats = log_mng_NewStats();

            if (pErr->Options.Advanced && !pErr->pAdvancedMng)
                pErr->pAdvancedMng = log_mng_NewAdvancedMng();
        }
        else
        {
            Res = LOG_MNG_ERROR_NOT_FOUND;
        }
    }
    else
    {
        Res = LOG_MNG_IS_NULL;
    }

    return Res;
}

log_mng_result_e log_mng_GetOptions(log_mng_t* pLogMng, int ErrId, log_mng_error_opt_t* pOpt)
{
    log_mng_result_e Res = LOG_MNG_SUCCESS;

    if (pLogMng)
    {
        int              Index;
        log_mng_error_t* pErr = log_mng_GetError(pLogMng, ErrId, &Index);

        if (pErr)
        {
            log_mng_error_opt_t* pOptCurrent = &pErr->Options;

            pOpt->ScratchPeriod = pOptCurrent->ScratchPeriod;
            pOpt->SilentLog     = pOptCurrent->SilentLog;
            pOpt->SilentLogTime = pOptCurrent->SilentLogTime;
            pOpt->UseStats      = pOptCurrent->UseStats;
            pOpt->Advanced      = pOptCurrent->Advanced;
        }
    }
    else
    {
        Res = LOG_MNG_ERROR_NOT_FOUND;
    }

    return Res;
}





/******************************************************
* LOG MANAGER FUNCTIONS
******************************************************/
log_mng_result_e log_mng_SetId(log_mng_t* pLogMng, int Id)
{
    log_mng_result_e res = LOG_MNG_SUCCESS;

    if (pLogMng)
        pLogMng->Id = Id;
    else
        res = LOG_MNG_OPERATION_FAILED;

    return res;
}


log_mng_result_e log_mng_SetAdvUserFunction(log_mng_t* pLogMng, int ErrId, log_mng_HandleDataCb fFun)
{
    log_mng_result_e Res = LOG_MNG_SUCCESS;
    if (pLogMng)
    {
        int              Index = -1;
        log_mng_error_t* pErr  = log_mng_GetError(pLogMng, ErrId, &Index);

        if (pErr)
        {
            if (pErr->pAdvancedMng)
                pErr->pAdvancedMng->fHandleData = fFun;
        }
        else
        {
            Res = LOG_MNG_ERROR_NOT_FOUND;
        }
    }

    return Res;
}


/******************************************************
* LOG MANAGER DIAGNOSTIC FUNCTIONS
******************************************************/

log_mng_result_e log_mng_SetDiagnostics(log_mng_t* pLogMng, int ErrId, log_mng_Diagnostics fFun)
{
    log_mng_result_e Res = LOG_MNG_SUCCESS;

    if (pLogMng)
    {
        int              Index = -1;
        log_mng_error_t* pErr  = log_mng_GetError(pLogMng, ErrId, &Index);

        if (pErr)
            pErr->fDiagnostics = fFun;
    }
    else
    {
        Res = LOG_MNG_ERROR_NOT_FOUND;
    }

    return Res;
}


void log_mng_GetArgInt(va_list* ArgList, int* pD)
{
    *pD = va_arg(*ArgList, int);
}

void log_mng_GetArgChar(va_list* ArgList, char* pC)
{
    *pC = (char) va_arg(*ArgList, int);
}

void log_mng_GetArgString(va_list* ArgList, char** pS)
{
    *pS = va_arg(*ArgList, char*);
}

void log_mng_GetArgUint(va_list* ArgList, unsigned int* pD)
{
    *pD = va_arg(*ArgList, unsigned int);
}


void log_mng_GetArgUlong(va_list* ArgList, unsigned long* pD)
{
    *pD = va_arg(*ArgList, unsigned long);
}


/******************************************************
* LOG MANAGER ERROR FUNCTIONS
******************************************************/

log_mng_error_t* log_mng_GetError(log_mng_t* pLogMng, int ErrorId, int* Index)
{
    log_mng_error_t* pErr       = NULL;
    int              i;
    int              ErrorIndex = -1;

    if (pLogMng)
    {
        if (ErrorId < pLogMng->NofErrors && pLogMng->Errors[ErrorId].ErrorId == ErrorId)
        {
            ErrorIndex = ErrorId;
        }
        else
        {
            /* Find the error in the error register */
            for (i = 0; i < pLogMng->NofErrors; i++)
            {
                if (pLogMng->Errors[i].ErrorId == ErrorId)
                {
                    ErrorIndex = i;
                    break;
                }
            }
        }
        if (ErrorIndex != -1)
        {
            pErr = &pLogMng->Errors[ErrorIndex];
            if (Index)
                *Index = ErrorIndex;
        }
    }

    return pErr;
}

log_mng_result_e log_mng_NewError(log_mng_t* pLogMng, int ErrorId, const char* pFormat)
{
    log_mng_result_e res    = LOG_MNG_SUCCESS;
    time_t           t;
    unsigned int     NowUtc = (unsigned int) time(&t);

    if (pLogMng)
    {
        /* Check if there is an error with this ID in the log manager */
        int ErrorIndex = -1;

        /* Find the error in the error register */
        if (log_mng_GetError(pLogMng, ErrorId, &ErrorIndex))
        {
            res = LOG_MNG_ERROR_ALREADY_EXISTS;
        }
        else
        {
            /* New error */
            if (pLogMng->NofErrors < LOG_MNG_MAX_ERRORS)
            {
                /* There is room for another error */
                ErrorIndex                               = pLogMng->NofErrors;

                pLogMng->Errors[ErrorIndex].ErrorId      = ErrorId;
                pLogMng->Errors[ErrorIndex].LastUpdate   = NowUtc;
                pLogMng->Errors[ErrorIndex].fDiagnostics = NULL;
                Srv_Strncpy(pLogMng->Errors[ErrorIndex].Error, pFormat, sizeof(pLogMng->Errors[ErrorIndex].Error));

                /* Set error options to default values */
                pLogMng->Errors[ErrorIndex].Options.ScratchPeriod   = LOG_MNG_SCRATCH_PERIOD;
                pLogMng->Errors[ErrorIndex].Options.SilentLog       = LOG_MNG_SILENT_LOG;
                pLogMng->Errors[ErrorIndex].Options.SilentLogTime   = LOG_MNG_SILENT_LOG_TIME;
                pLogMng->Errors[ErrorIndex].Options.ShowFirstN      = 0;
                pLogMng->Errors[ErrorIndex].Options.UseStats        = LOG_MNG_STATS_OFF;
                pLogMng->Errors[ErrorIndex].Options.Advanced        = LOG_MNG_ADVANCED_OFF;
                pLogMng->Errors[ErrorIndex].Options.AdvancedData    = LOG_MNG_ADVANCED_DATA_SAVE;
                pLogMng->Errors[ErrorIndex].Options.AdvancedDataMax = LOG_MNG_ADVANCED_DATA_MAX;
                pLogMng->Errors[ErrorIndex].Options.Diagnostic      = LOG_MNG_DIAGNOSTIC_RUN_ON_DISPLAY;

                pLogMng->NofErrors++;
            }
            else
            {
                /* error register exhausted */
                res = LOG_MNG_ERROR_REGISTER_EXHAUSTED;
            }
        }
    }
    else
    {
        /* pLogMng is NULL */
        res = LOG_MNG_IS_NULL;
    }

    return res;
}

static void log_mng_PrintFirst(App_ModuleId_t ModId, Log::Level_t Level, char* str, int iNofChar)
{
    // if -1 was returned as iNofChar, the whole buffer str was filled i.e. sizeof(str) chars were written. TR 13038
    if (iNofChar == -1 || iNofChar >= LOG_MNG_MAX_LOG_STRING)
    {
        iNofChar = LOG_MNG_MAX_LOG_STRING;
        Srv_Strncpy(str + LOG_MNG_MAX_LOG_STRING - 5, " ...", 5);    // Show that the string was truncated by setting the end to " ...\0"
    }

    App_Log(Level, ModId, "%s", str);
}


static void log_mng_PrintPeriod(App_ModuleId_t ModId, Log::Level_t Level, char* str, int iNofChar, int NofRep, int ShowExtention)
{
    // if -1 was returned as iNofChar, the whole buffer str was filled i.e. sizeof(str) chars were written. TR 13038
    if (iNofChar == -1 || iNofChar >= LOG_MNG_MAX_LOG_STRING)
    {
        iNofChar = LOG_MNG_MAX_LOG_STRING;
        Srv_Strncpy(str + LOG_MNG_MAX_LOG_STRING - 5, " ...", 5);    // Show that the string was truncated by setting the end to " ...\0"
    }

    if (ShowExtention)
        Srv_Snprintf(str + iNofChar, LOG_MNG_MAX_LOG_STRING - iNofChar, ", (error occurred %d times)", NofRep);

    App_Log(Level, ModId, "%s", str);
}



/******************************************************
* LOG MANAGER MAIN FUNCTION
******************************************************/

static void log_mng_ModLogMask(Log::Mask_t* LogMaskReg)
{
    int i;
    /* NOTE: This code is not in use */
    for (i = APP_ID; i < LAST_ID; i++)
    {
        LogMaskReg[i] = Logging_getLogMask((App_ModuleId_t)i);
        Logging_setLogMask((App_ModuleId_t)i, Log::DebugMask);
    }
}

static void log_mng_RestoreLogMask(Log::Mask_t* LogMaskReg)
{
    int i;
    /* NOTE: This code is not in use */
    for (i = APP_ID; i < LAST_ID; i++)
    {
        Logging_setLogMask((App_ModuleId_t)i, LogMaskReg[i]);
    }
}

__attribute__ ((format(printf, 4, 0)))
int logCreateLogString(log_mng_logCb logCb, const void* pArg, char* logString, const char* Format, va_list ArgList)
{
    if (logCb && pArg)
    {
        return logCb(logString, pArg, Format, ArgList);
    }
    return Srv_Vsnprintf(logString, LOG_MNG_MAX_LOG_STRING, Format, ArgList);
}

log_mng_result_e log_mng_log(log_mng_t* pLogMng, Log::Level_t Level, App_ModuleId_t ModuleId, int ErrorId,  const char* Format, ...)
{
    va_list          ArgList;
    log_mng_result_e res;
    va_start(ArgList, Format);
    res = log_mng_log2(pLogMng, Level, ModuleId, ErrorId, NULL, NULL, Format, ArgList);
    va_end(ArgList);
    return res;
}

__attribute__ ((format(printf, 7, 8)))
log_mng_result_e log_mng_log4(log_mng_t* pLogMng, Log::Level_t Level, App_ModuleId_t ModuleId, int ErrorId, log_mng_logCb logCb, void* pArg, const char* Format, ...)
{
    va_list          ArgList;
    log_mng_result_e res;
    va_start(ArgList, Format);
    res = log_mng_log2(pLogMng, Level, ModuleId, ErrorId, logCb, pArg, Format, ArgList);
    va_end(ArgList);
    return res;
}

log_mng_result_e log_mng_log3(log_mng_t* pLogMng, Log::Level_t Level, App_ModuleId_t ModuleId, int ErrorId, log_mng_logCb logCb, void* pArg)
{
    log_mng_result_e res;
    res = log_mng_log4(pLogMng, Level, ModuleId, ErrorId, logCb, pArg, "%s", "");
    return res;
}

log_mng_result_e log_mng_log2(log_mng_t* pLogMng, Log::Level_t Level, App_ModuleId_t ModuleId, int ErrorId,  log_mng_logCb logCb, const void* pArg, const char* Format, va_list ArgList)
{
    log_mng_result_e result = LOG_MNG_SUCCESS;

    /* Check log level. Continue only if set */
    if (Logging_isLogEnabled(ModuleId, Level))
    {
        log_mng_error_t* pErr          = NULL;
        int              iDiagnostics  = 0;
        int              RunDiagnostic = 0;
        int              SaveData      = 0;
        int              ShowLog       = 0;
        int              ShowExtention = 1;
        int              iIndex;
        time_t           NowUtc;
        int              iNofChar;

        struct timeval   TimeNow = monotonic_time_get_timeval();

        NowUtc = TimeNow.tv_sec;


        /* Begin by finding the error in the error register.
           If the error does not exists create a new one.
         */
        pErr = log_mng_GetError(pLogMng, ErrorId, &iIndex);

        if (!pErr)
        {
            log_mng_result_e ErrCreate = log_mng_NewError(pLogMng, ErrorId, Format);
            if (ErrCreate != LOG_MNG_SUCCESS)
            {
                result = ErrCreate;
                return result;
            }
            else
            {
                pErr = log_mng_GetError(pLogMng, ErrorId, &iIndex);
                APP_ASSERT(pErr);
            }
        }

        /* If the error has just been created, or has been created manually but not used,
           then some values need to be set */
        if (pErr->ErrorSet == 0)
        {
            /* This is a new error. Initialize values */
            log_mng_StatsUpdateFirst(pErr, NowUtc);
            pErr->ErrorSet = 1;
        }

        pErr->NofRep++;
        log_mng_StatsUpdateTotal(pErr);

        /* If the error hans't been reported for a whole scratch period it is considered
           new, as if occuring for the first time. We forget history and start from scratch*/
        if (((time_t)(NowUtc - pErr->LastUpdate)) > pErr->Options.ScratchPeriod)
        {
            pErr->nThreshold  = 0;
            pErr->LastPrinted = 0;
            if (pErr->NofRep > 1)
            {
                pErr->NofRep = 1;
                log_mng_StatsUpdateScratch(pErr);
            }
        }

        /* Update the time stamp */
        pErr->LastUpdate = NowUtc;
        log_mng_StatsUpdateLast(pErr, NowUtc);

        ShowExtention = (pErr->Options.SilentLog > 1) || (pErr->Options.SilentLogTime > 0);

        /* If this is the first occurrence of the error, or if it has been logged
           the maximum number of times between prints, display it
         */
        ShowLog =   (pErr->NofRep  == 1) ||
                  (pErr->NofRep  <= pErr->Options.ShowFirstN) ||
                  ((pErr->NofRep >= (pErr->nThreshold)) &&
                   (((uint32_t)(NowUtc - pErr->LastPrinted)) >= pErr->Options.SilentLogTime));

        if (ShowLog)
        {
            static char logString[LOG_MNG_MAX_LOG_STRING];
            iNofChar = logCreateLogString(logCb, pArg, (char*)&logString, Format, ArgList);
            APP_ASSERT((iNofChar <= LOG_MNG_MAX_LOG_STRING));

            if (pErr->NofRep == 1)
            {
                log_mng_PrintFirst(ModuleId, Level, logString, iNofChar);
                pErr->nThreshold = pErr->Options.SilentLog;
                iDiagnostics     = 1;
            }
            else
            {
                log_mng_PrintPeriod(ModuleId, Level, logString, iNofChar, pErr->NofRep, ShowExtention);
                pErr->nThreshold = pErr->NofRep + pErr->Options.SilentLog;
                if (pErr->Options.Diagnostic == LOG_MNG_DIAGNOSTIC_RUN_ON_DISPLAY)
                    iDiagnostics = 1;
            }

            pErr->LastPrinted = NowUtc;
        }

        /* Check is data needs to be saved */
        SaveData = (!logCb &&
                    (pErr->Options.Advanced && pErr->pAdvancedMng) &&
                    !(pErr->pAdvancedMng->SizeOfData > pErr->Options.AdvancedDataMax) &&
                    ((pErr->Options.AdvancedData == LOG_MNG_ADVANCED_DATA_SAVE) ||
                     ((pErr->Options.AdvancedData == LOG_MNG_ADVANCED_DATA_SAVE_FIRST && (pErr->NofRep == 1)))));

        if (SaveData)
        {
            int NofBytes;
            NofBytes                        = log_mng_SaveData(ArgList, Format, pErr->pAdvancedMng->pWrite);

            pErr->pAdvancedMng->pWrite     += NofBytes;
            pErr->pAdvancedMng->SizeOfData += NofBytes;
            pErr->pAdvancedMng->NofElements++;

            /* Check if data buffer exhausted */
            if ((pErr->pAdvancedMng->SizeOfBuffer - pErr->pAdvancedMng->SizeOfData) < (LOG_MNG_DATA_BLOCK_SIZE / 2))
            {
                /* Allocate one more data block */
                log_mng_ExtendAdvancedDataBuffer(pErr->pAdvancedMng);
            }
        }

        /* NOTE: Diagnostics can be removed, it is not used */
        RunDiagnostic = iDiagnostics || (pErr->Options.Diagnostic == LOG_MNG_DIAGNOSTIC_RUN_EACH_TIME);

        /* Apply user diagnostic function */
        /* NOTE: This is never run, because fDiagnostics is always NULL and never set */
        if (RunDiagnostic && pErr->fDiagnostics)
        {
            Log::Mask_t ModLogMask[LAST_ID];

            /* Modify all log masks to DEBUG */
            log_mng_ModLogMask(ModLogMask);

            (*pErr->fDiagnostics)(ArgList);

            /* Restore all log masks */
            log_mng_RestoreLogMask(ModLogMask);
        }
    }
    return result;
}


/******************************************************
* LOG MANAGER ADVANCED FUNCTIONS
******************************************************/

static log_mng_advanced_t* log_mng_NewAdvancedMng()
{
    log_mng_advanced_t* pAdvMng = new log_mng_advanced_t();

    App_Log(Log::Debug, SRVLIB_ID, "log_mng_NewAdvancedMng");

    pAdvMng->pData        = new char[LOG_MNG_DATA_BLOCK_SIZE];
    pAdvMng->pRead        = pAdvMng->pData;
    pAdvMng->pWrite       = pAdvMng->pData;
    pAdvMng->SizeOfData   = 0;
    pAdvMng->NofElements  = 0;
    pAdvMng->SizeOfBuffer = LOG_MNG_DATA_BLOCK_SIZE;
    pAdvMng->fHandleData  = NULL;

    return pAdvMng;
}


static int log_mng_ExtendAdvancedDataBuffer(log_mng_advanced_t* pAdvMng)
{
    char* pTmp    = NULL;
    int   NewSize = pAdvMng->SizeOfBuffer + LOG_MNG_DATA_BLOCK_SIZE;
    int   Res     = LOG_MNG_SUCCESS;

    App_Log(Log::Debug, SRVLIB_ID, "log_mng_ExtendAdvancedDataBuffer");

    pTmp = new char[NewSize]();
    memcpy(pTmp, pAdvMng->pData, pAdvMng->SizeOfData);
    delete[] pAdvMng->pData;
    pAdvMng->pData        = pTmp;
    pAdvMng->pRead        = pTmp;
    pAdvMng->pWrite       = pTmp + pAdvMng->SizeOfData;
    pAdvMng->SizeOfBuffer = NewSize;

    Res                   = NewSize;

    return Res;
}

log_mng_result_e log_mng_AdvInitDataIter(log_mng_t* pLogMng, int ErrId, log_mng_advanced_iter_t* pIt)
{
    log_mng_result_e Res  = LOG_MNG_SUCCESS;
    log_mng_error_t* pErr = NULL;
    int              Index;

    pErr = log_mng_GetError(pLogMng, ErrId, &Index);

    if (pErr)
    {
        if (pErr->Options.Advanced && pErr->pAdvancedMng)
        {
            pIt->pIt     = pErr->pAdvancedMng->pRead;
            pIt->MaxSize = pErr->pAdvancedMng->SizeOfData;
            pIt->Size    = 0;
        }
    }
    else
    {
        Res = LOG_MNG_ERROR_NOT_FOUND;
    }

    return Res;
}

int log_mng_AdvGetString(log_mng_advanced_iter_t* pIt, char* pStr)
{
    char* pBuf = pIt->pIt;
    int   Len  = strlen(pBuf) + 1;      /* include the end-of-line */

    if (pIt->Size + Len > pIt->MaxSize)
    {
        Len = -1;
    }
    else
    {
        memcpy(pStr, pBuf, Len);
        pIt->pIt  += Len;
        pIt->Size += Len;
    }

    return Len;
}

int log_mng_AdvGetInt(log_mng_advanced_iter_t* pIt, int* pDout)
{
    int* pD  = (int*)pIt->pIt;
    int  Len = sizeof(*pD);

    if (pIt->Size + Len > pIt->MaxSize)
    {
        Len = -1;
    }
    else
    {
        *pDout     = *pD;
        pIt->pIt  += Len;
        pIt->Size += Len;
    }

    return Len;
}

int log_mng_AdvGetChar(log_mng_advanced_iter_t* pIt, char* pCout)
{
    char* pC  = (char*)pIt->pIt;
    int   Len = sizeof(*pC);

    if (pIt->Size + Len > pIt->MaxSize)
    {
        Len = -1;
    }
    else
    {
        *pCout     = *pC;
        pIt->pIt  += Len;
        pIt->Size += Len;
    }

    return Len;
}

int log_mng_AdvGetUInt(log_mng_advanced_iter_t* pIt, unsigned int* pUout)
{
    unsigned int* pU  = (unsigned int*)pIt->pIt;
    int           Len = sizeof(*pU);

    if (pIt->Size + Len > pIt->MaxSize)
    {
        Len = -1;
    }
    else
    {
        *pUout     = *pU;
        pIt->pIt  += Len;
        pIt->Size += Len;
    }

    return Len;
}

int log_mng_AdvGetULong(log_mng_advanced_iter_t* pIt, unsigned long* pLout)
{
    unsigned long* pL  = (unsigned long*)pIt->pIt;
    int            Len = sizeof(*pL);

    if (pIt->Size + Len > pIt->MaxSize)
    {
        Len = -1;
    }
    else
    {
        *pLout     = *pL;
        pIt->pIt  += Len;
        pIt->Size += Len;
    }

    return Len;
}


log_mng_result_e log_mng_DeleteData(log_mng_t* pLogMng, int ErrId)
{
    log_mng_result_e Res = LOG_MNG_SUCCESS;

    if (pLogMng)
    {
        int              Index = -1;
        log_mng_error_t* pErr  = log_mng_GetError(pLogMng, ErrId, &Index);

        if (pErr)
        {
            if (pErr->pAdvancedMng)
            {
                /* Zero all data that has been saved */
                pErr->pAdvancedMng->pRead       = pErr->pAdvancedMng->pData;
                memset(pErr->pAdvancedMng->pRead, 0, pErr->pAdvancedMng->pWrite - pErr->pAdvancedMng->pRead);
                pErr->pAdvancedMng->pWrite      = pErr->pAdvancedMng->pRead;
                pErr->pAdvancedMng->SizeOfData  = 0;
                pErr->pAdvancedMng->NofElements = 0;
            }
        }
        else
        {
            Res = LOG_MNG_ERROR_NOT_FOUND;
        }
    }

    return Res;
}


static int log_mng_SaveData(va_list ap, const char* fmt, char* buf)
{
    int            d;
    int            NofBytes = 0;
    char           c, * s;
    unsigned int   u;
    unsigned long  lu;
    uint64_t       llu;
    char*          buf_p = buf;
    int            len;
    int*           d_p;
    unsigned int*  u_p;
    unsigned long* lu_p;
    uint64_t*      llu_p;
    time_t         t;
    unsigned int   NowUtc = (unsigned int) time(&t);

    char*          c_p;

    u_p       = (unsigned int*)buf_p;
    *u_p      = NowUtc;
    buf_p     = (char*)(buf_p + sizeof(unsigned int));
    NofBytes += sizeof(unsigned int);

    while (*fmt)
    {
        switch (*fmt++)
        {
            case '%':
            {
                while ((*fmt == '-') || (*fmt == '+') || (*fmt == '#') || (*fmt == '0'))
                {
                    fmt++;
                }

                if ((*fmt >= '1') && (*fmt <= '9'))
                {
                    int Acc = 0;
                    while ((*fmt > '0') && (*fmt < '9'))
                    {
                        Acc += (Acc * 10) + (*fmt - 30);
                        fmt++;
                    }
                }

                switch (*fmt)
                {
                    case 's':       /* string */
                        s = va_arg(ap, char*);

                        if ((strlen(s) % 2) != 0)
                            len = Srv_Snprintf(buf_p, 255, "%s%c", s, 32);
                        else
                            len = Srv_Snprintf(buf_p, 255, "%s", s);

                        buf_p     = (char*)(buf_p + len + 1);
                        NofBytes += len + 1;
                        break;
                    case 'd':       /* int */
                        d         = va_arg(ap, int);
                        d_p       = (int*)buf_p;
                        *d_p      = d;
                        buf_p     = (char*)(buf_p + sizeof(*d_p));
                        NofBytes += sizeof(*d_p);
                        break;
                    case 'u':       /* unsigned int */
                        u         = va_arg(ap, unsigned int);
                        u_p       = (unsigned int*)buf_p;
                        *u_p      = u;
                        buf_p     = (char*)(buf_p + sizeof(*u_p));
                        NofBytes += sizeof(*u_p);
                        break;
                    case 'l':       /* unsigned long */
                        if (*(fmt + 1) == 'u')
                        {
                            lu        = va_arg(ap, unsigned long);
                            lu_p      = (unsigned long*)buf_p;
                            *lu_p     = lu;
                            buf_p     = (char*)(buf_p + sizeof(*lu_p));
                            NofBytes += sizeof(*lu_p);
                        }

                        if (*(fmt + 1) == 'l')
                        {
                            if (*(fmt + 2) == 'u')
                            {
                                llu       = va_arg(ap, uint64_t);
                                llu_p     = (uint64_t*)buf_p;
                                *llu_p    = llu;
                                buf_p     = (char*)(buf_p + sizeof(*llu_p));
                                NofBytes += sizeof(*llu_p);
                            }
                        }
                        break;
                    case 'c':       /* char */
                        c     = (char) va_arg(ap, int);
                        c_p   = buf_p;
                        *c_p  = c;
                        buf_p = (char*)(buf_p + sizeof(*c_p));
                        NofBytes++;
                        break;
                    case 'x':       /* hex */
                    case 'X':       /* hex */
                        d         = va_arg(ap, int);
                        d_p       = (int*)buf_p;
                        *d_p      = d;
                        buf_p     = (char*)(buf_p + sizeof(*d_p));
                        NofBytes += sizeof(*d_p);
                        break;
                }
            }
        }
    }

    return NofBytes;
}
