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

#include "app/include/app_memtracker.h"
#include "memtracker.h"
#include "core/include/core.h"
#include "oamframework/include/oam_srv_api.h"
#include "platform/include/util/board_attr.h"
#include "srvlib/include/timezone.h"
#include "srvlib/include/backtrace.h"
#include "srvlib/include/srvutil.h"
#include "srvlib/include/thread.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <time.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdtypes.h>

#if defined(__linux__)
#include <execinfo.h>
#endif

#if defined(GGSN_SEOS)
#define BASE_PATH "/var/log/services/epg/memtracker"
#else
#define BASE_PATH "/var/log/memtracker"
#endif

#define MEMTRACKER_GSC "gsc"
#define MEMTRACKER_PSC "psc"
#define MEMTRACKER_START_ALL "all"

#define BT_CALLBACK_LEN     6

enum memtracker_status_t
{
    MEMTRACKER_STATUS_STOPPED,
    MEMTRACKER_STATUS_RUNNING,
    MEMTRACKER_STATUS_GENERATING_REPORT,
    MEMTRACKER_STATUS_SHUTDOWN,
};

static memtracker_status_t app_memtracker_status;

static FILE*               fp;
static uint32_t            report_file_serial_no;

/*--------------------------------------------------------------------------------------------------------------------*/


static void create_report_file_name(const struct tm* ltime, char* name, const uint32_t len)
{
    char        timezone[10];
    Board_Id_t  board;
    const char* context = MEMTRACKER_PSC;
    BoardAttr_getId(&board);

    if (Core_GetExecutionContext() == APP_GSC_CTX)
    {
        context = MEMTRACKER_GSC;
    }
    Timezone_getGMTOffsetAsStr(timezone, 10, false);
    Srv_Snprintf(name, len, "%s/memtracker_report_%s_%u_%u_%u_%d%.2d%.2d.%.2d%.2d%s-%02x.txt", BASE_PATH, context,
            board.BoardGroupSlot, board.BoardSlot, App_GetInstanceId(),
            ltime->tm_year + 1900, ltime->tm_mon + 1, ltime->tm_mday, ltime->tm_hour, ltime->tm_min, timezone,
            report_file_serial_no++);
}

/*--------------------------------------------------------------------------------------------------------------------*/

static int create_report_directory()
{
    struct stat st;
    if (stat(BASE_PATH, &st) == -1)
    {
        if (mkdir(BASE_PATH, 0755) == -1)
        {
            App_Log(Log::Warning, APP_ID,  "%s: Creating report directory failed. Errno =  %s\n",  __FUNCTION__, strerror(errno));
            return EFAIL;
        }
    }
    return EOK;
}

/*--------------------------------------------------------------------------------------------------------------------*/

static bool report_file_open()
{
    char             rep_file[PATH_MAX];
    const time_t     t     = time(NULL);
    struct tm        resultTime;
    const struct tm* ltime = localtime_r(&t, &resultTime);

    if (create_report_directory() != EOK)
    {
        return false;
    }

    create_report_file_name(ltime, rep_file, PATH_MAX);
    fp = fopen(rep_file, "w");
    if (NULL == fp)
    {
        App_Log(Log::Error, APP_ID, "%s: Opening memtracker report file %s failed\n. Errno = %s",  __FUNCTION__, rep_file, strerror(errno));
        return false;
    }
    else
    {
        App_Log(Log::Notice, APP_ID, "%s: Opening memtracker report file %s \n",  __FUNCTION__, rep_file);
    }
    return true;
}

/*--------------------------------------------------------------------------------------------------------------------*/

static void report_file_close(void)
{
    if (fp)
    {
        fclose(fp);
    }
}

/*--------------------------------------------------------------------------------------------------------------------*/

static void report_file_print(const char* alloc)
{
    if (fp)
    {
        fprintf(fp, "%s", alloc);
    }
}

/*--------------------------------------------------------------------------------------------------------------------*/

static void app_report_generator(void* arg)
{
    MemEntryIterator_t* iter = MemTracker_CreateMemEntryIterator();
    void*               elem;
    char*               buf;

    (void)arg;

    while ((elem = MemTracker_MemEntryIterGetNext(iter)) != NULL)
    {
        if ((buf = MemTracker_ElemToStr(elem)) != NULL)
        {
            report_file_print(buf);
        }
    }
    report_file_close();
    MemTracker_DeleteMemEntryIterator(iter);
    app_memtracker_status = MEMTRACKER_STATUS_RUNNING;
}

/*--------------------------------------------------------------------------------------------------------------------*/

static void app_memtracker_flush(void* arg)
{
    (void)arg;

    MemTracker_Stop();
    Core_Watchdog_Enable();
    app_memtracker_status = MEMTRACKER_STATUS_STOPPED;
}

/*--------------------------------------------------------------------------------------------------------------------*/

static bool cli_for_myboard(const char* board)
{
    const char* my_board = BoardAttr_getBoardName();
    return strncmp(board, my_board, strlen(board)) == 0;
}

/*--------------------------------------------------------------------------------------------------------------------*/

static void cli_response_start(OAM_SrvCLIRequest_t* pCLIReq)
{
    OAM_SrvAppendRespData(pCLIReq, "<output>\n");
}

/*--------------------------------------------------------------------------------------------------------------------*/

static void cli_response_end(OAM_SrvCLIRequest_t* pCLIReq)
{
    OAM_SrvAppendRespData(pCLIReq, "\n");
    OAM_SrvAppendRespData(pCLIReq, "</output>\n");
    pCLIReq->RespStatus = OAM_SrvSuccessXMLResp;
    OAM_SrvReturnCLIResponse(pCLIReq);
}

/*--------------------------------------------------------------------------------------------------------------------*/

static bool app_memtracker_start(void)
{
    if (MemTracker_IsStarted())
    {
        return false;
    }
    /* Running watchdog while running memtracker may cause coredump on  Linux if signalhandedler is called while in backtrace */
    Core_Watchdog_Disable();
    App_Log(Log::Notice, APP_ID, "%s",  __FUNCTION__);
    MemTracker_Start();
    return true;
}

/*--------------------------------------------------------------------------------------------------------------------*/

static bool app_memtracker_stop(void)
{
    srvlib::Thread thread;
    thread.start(app_memtracker_flush, NULL);
    thread.detach();

    App_Log(Log::Notice, APP_ID, "%s",  __FUNCTION__);
    return true;
}

/*--------------------------------------------------------------------------------------------------------------------*/

static bool app_memtracker_generatereport(const bool checkpoint __attribute__ ((unused)))
{
    if (!report_file_open())
    {
        return false;
    }

    srvlib::Thread thread;
    thread.start(app_report_generator, NULL);
    thread.detach();

    App_Log(Log::Notice, APP_ID, "%s",  __FUNCTION__);
    return true;
}

/*--------------------------------------------------------------------------------------------------------------------*/

static int memtracker_get_cpuid(void)
{
    return 0;
}

/*--------------------------------------------------------------------------------------------------------------------*/

static void processMemtrackerStartCommand(OAM_SrvCLIRequest_t* pCLIReq)
{
    const char* board = pCLIReq->ArgStr[0];

    if (!cli_for_myboard(board))
    {
        pCLIReq->RespStatus = OAM_SrvSuccessXMLResp;
        OAM_SrvReturnCLIResponse(pCLIReq);
        return;
    }

    cli_response_start(pCLIReq);

    switch (app_memtracker_status)
    {
        case MEMTRACKER_STATUS_STOPPED:
            if (app_memtracker_start())
            {
                app_memtracker_status = MEMTRACKER_STATUS_RUNNING;
                report_file_serial_no = 0;
                OAM_SrvAppendRespData(pCLIReq, "Memtracker started.");
            }
            else
            {
                OAM_SrvAppendRespData(pCLIReq, "Failure starting memtracker.");
            }
            break;
        case MEMTRACKER_STATUS_RUNNING:
        case MEMTRACKER_STATUS_GENERATING_REPORT:
            OAM_SrvAppendRespData(pCLIReq, "Memtracker already running.");
            break;
        case MEMTRACKER_STATUS_SHUTDOWN:
            OAM_SrvAppendRespData(pCLIReq, "Memtracker is shutting down.");
            break;
        default:
            OAM_SrvAppendRespData(pCLIReq, "Unknown memtracker fault");
    }

    cli_response_end(pCLIReq);
}

/*--------------------------------------------------------------------------------------------------------------------*/

static void processMemtrackerStopCommand(OAM_SrvCLIRequest_t* pCLIReq)
{
    const char* board = pCLIReq->ArgStr[0];

    if (!cli_for_myboard(board))
    {
        pCLIReq->RespStatus = OAM_SrvSuccessXMLResp;
        OAM_SrvReturnCLIResponse(pCLIReq);
        return;
    }

    cli_response_start(pCLIReq);

    switch (app_memtracker_status)
    {
        case MEMTRACKER_STATUS_STOPPED:
            OAM_SrvAppendRespData(pCLIReq, "Memtracker is not started.");
            break;
        case MEMTRACKER_STATUS_RUNNING:
            app_memtracker_stop();
            app_memtracker_status = MEMTRACKER_STATUS_SHUTDOWN;
            OAM_SrvAppendRespData(pCLIReq, "Stopping memtracker.");
            break;
        case MEMTRACKER_STATUS_GENERATING_REPORT:
            OAM_SrvAppendRespData(pCLIReq, "Memtracker busy generating report.");
            break;
        case MEMTRACKER_STATUS_SHUTDOWN:
            OAM_SrvAppendRespData(pCLIReq, "Memtracker is shutting down.");
            break;
        default:
            OAM_SrvAppendRespData(pCLIReq, "Unknown memtracker fault");
    }

    cli_response_end(pCLIReq);
}

/*--------------------------------------------------------------------------------------------------------------------*/


static void processMemtrackerReportCommand(OAM_SrvCLIRequest_t* pCLIReq)
{
    const char* board = pCLIReq->ArgStr[0];

    if (!cli_for_myboard(board))
    {
        pCLIReq->RespStatus = OAM_SrvSuccessXMLResp;
        OAM_SrvReturnCLIResponse(pCLIReq);
        return;
    }

    cli_response_start(pCLIReq);

    switch (app_memtracker_status)
    {
        case MEMTRACKER_STATUS_STOPPED:
            OAM_SrvAppendRespData(pCLIReq, "Memtracker is not started.");
            break;
        case MEMTRACKER_STATUS_RUNNING:
            if (app_memtracker_generatereport(true))
            {
                OAM_SrvAppendRespData(pCLIReq, "Generating memtracker report.");
                app_memtracker_status = MEMTRACKER_STATUS_GENERATING_REPORT;
            }
            else
            {
                OAM_SrvAppendRespData(pCLIReq, "Failure Generating memtracker report.");
            }
            break;
        case MEMTRACKER_STATUS_GENERATING_REPORT:
            OAM_SrvAppendRespData(pCLIReq, "Memtracker busy generating report.");
            break;
        case MEMTRACKER_STATUS_SHUTDOWN:
            OAM_SrvAppendRespData(pCLIReq, "Memtracker is shutting down. Report not generated");
            break;
        default:
            OAM_SrvAppendRespData(pCLIReq, "Unknown memtracker fault");
    }

    cli_response_end(pCLIReq);
}

/*--------------------------------------------------------------------------------------------------------------------*/

static void processMemtrackerShowCommand(OAM_SrvCLIRequest_t* pCLIReq)
{
    const char* board = pCLIReq->ArgStr[0];

    if (!cli_for_myboard(board))
    {
        pCLIReq->RespStatus = OAM_SrvSuccessXMLResp;
        OAM_SrvReturnCLIResponse(pCLIReq);
        return;
    }

    cli_response_start(pCLIReq);

    switch (app_memtracker_status)
    {
        case MEMTRACKER_STATUS_STOPPED:
            OAM_SrvAppendRespData(pCLIReq, "Memtracker is not started.");
            break;
        case MEMTRACKER_STATUS_RUNNING:
            OAM_SrvAppendRespData(pCLIReq, "Memtracker is running.");
            break;
        case MEMTRACKER_STATUS_GENERATING_REPORT:
            OAM_SrvAppendRespData(pCLIReq, "Memtracker is running. Generating report.");
            break;
        case MEMTRACKER_STATUS_SHUTDOWN:
            OAM_SrvAppendRespData(pCLIReq, "Memtracker is shutting down.");
            break;
        default:
            OAM_SrvAppendRespData(pCLIReq, "Unknown memtracker fault.");
    }

    cli_response_end(pCLIReq);
}

/*--------------------------------------------------------------------------------------------------------------------*/

static void registerOamCallbacks()
{
    OAM_SrvRegisterCallback(OAM_SRV_MEMTRACKER_START_REQ_ID, processMemtrackerStartCommand);
    OAM_SrvRegisterCallback(OAM_SRV_MEMTRACKER_STOP_REQ_ID, processMemtrackerStopCommand);
    OAM_SrvRegisterCallback(OAM_SRV_MEMTRACKER_REPORT_REQ_ID, processMemtrackerReportCommand);
    OAM_SrvRegisterCallback(OAM_SRV_MEMTRACKER_SHOW_REQ_ID, processMemtrackerShowCommand);
}

/*--------------------------------------------------------------------------------------------------------------------*/

void memTrackerInit()
{
    MemTracker_t*     pGlobals = NULL;
    MemTracker_CbFn_t callbacks;

    callbacks.alloc         = __real_malloc;
    callbacks.calloc        =  __real_calloc;
    callbacks.zalloc        =  NULL;
    callbacks.free          =  __real_free;
    callbacks.backtrace     = backtrace;
    callbacks.tracked_alloc = NULL;

    registerOamCallbacks();

    MemTracker_Init(pGlobals, callbacks, BT_CALLBACK_LEN);
}

/*--------------------------------------------------------------------------------------------------------------------*/

/* Wrappers for C++ new/delete */

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__linux__)

extern void* __real__Znwm(size_t size);

void* __wrap__Znwm(size_t size)
{
    void* pMemory = __real__Znwm(MemTracker_GetSizeIncludingRedZones(size));
    if (NULL == pMemory)
        return NULL;
    if (MemTracker_IsStarted())
    {
        void* CallStack[BT_CALLBACK_LEN];
        int   nStackSize;
        nStackSize = backtrace(CallStack, BT_CALLBACK_LEN);
        MemTracker_Alloc_helper(pMemory, size, memtracker_get_cpuid(), CallStack, nStackSize);
    }
    return pMemory;
}

extern void* __real__Znam(size_t size);

void* __wrap__Znam(size_t size)
{
    void* pMemory = __real__Znam(MemTracker_GetSizeIncludingRedZones(size));
    if (NULL == pMemory)
        return NULL;
    if (MemTracker_IsStarted())
    {
        void* CallStack[BT_CALLBACK_LEN];
        int   nStackSize;
        nStackSize = backtrace(CallStack, BT_CALLBACK_LEN);
        MemTracker_Alloc_helper(pMemory, size, memtracker_get_cpuid(), CallStack, nStackSize);
    }
    return pMemory;
}

#else /* !__linux__ */

extern void* __real__Znwj(size_t size);

void* __wrap__Znwj(size_t size)
{
    void* pUserMemory;
    void* pMemory = __real__Znwj(MemTracker_GetSizeIncludingRedZones(size));
    if (NULL == pMemory)
        return NULL;
    pUserMemory = MemTracker_Alloc_helper(pMemory, size, memtracker_get_cpuid());
    return pUserMemory;
}

extern void* __real__Znaj(size_t size);

void* __wrap__Znaj(size_t size)
{
    void* pUserMemory;
    void* pMemory = __real__Znaj(MemTracker_GetSizeIncludingRedZones(size));
    if (NULL == pMemory)
        return NULL;
    pUserMemory = MemTracker_Alloc_helper(pMemory, size, memtracker_get_cpuid());
    return pUserMemory;
}

#endif /* !__linux__ */

extern void __real__ZdlPv(void* pUserMemory);

void __wrap__ZdlPv(void* pUserMemory)
{
    void* pMemory = MemTracker_FreeHelper(pUserMemory, memtracker_get_cpuid());
    __real__ZdlPv(pMemory);
}

extern void __real__ZdaPv(void* pUserMemory);

void __wrap__ZdaPv(void* pUserMemory)
{
    void* pMemory = MemTracker_FreeHelper(pUserMemory, memtracker_get_cpuid());
    __real__ZdaPv(pMemory);
}

#ifdef __cplusplus
}
#endif
