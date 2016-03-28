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

/* --- Includes --- */

#include "app/include/app.h"
#include "app/include/app_global.h"
#include "app_local.h"
#include "core/include/core.h"
#include "oamframework/include/oam_srv_api.h"
#include "srvlib/include/srvutil.h"
#include "platform/include/util/board_attr.h"

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <poll.h>
#include <sys/resource.h>

#if defined(WITH_JBMALLOC)
#include "jbmalloc.h"
#endif

/* ======================================================================
** Internal functions
** ====================================================================*/

#ifndef FDHCPD

int PrintMallocStats(OAM_SrvCLIRequest_t* pCLIReq, int print_detail)
{
    char buf[4000];

    (void)print_detail;

#if defined(WITH_JBMALLOC)
    int r = jbmalloc_stats(buf, sizeof(buf));
    if (r < 0)
    {
        r = 0;
    }
    else if (r >= (int)sizeof(buf))
    {
        r = sizeof(buf) - 1;
    }
    buf[r] = '\0';
#else
    buf[0] = '\0';
#endif

    if (buf[0] == '\0')
    {
        OAM_SrvAppendRespData(pCLIReq, "\nHeap statistics unavailable\n\n");
    }
    else
    {
        OAM_SrvAppendRespData(pCLIReq, "\nHeap statistics:\n\n");
        OAM_SrvAppendRespData(pCLIReq, buf);
    }

    return 1;
}

/* Syntax: mallocstats [detail]
 *         Prints statistics from the memory allocator.
 */
static bool processMallocStatsCommand(OAM_SrvCLIRequest_t* pCLIReq)
{
    char* str          = pCLIReq->ArgStr[0];
    int   print_detail = 0;
    if (!str)
        return false;

    Srv_EatStrSpaces(str);
    if (strncasecmp(str, "show mallocstats", 16))
        return false;

    OAM_SrvAppendRespData(pCLIReq, "<output>\n");
    print_detail = strcasecmp(str, "show mallocstats detail") == 0;
    PrintMallocStats(pCLIReq, print_detail);
    OAM_SrvAppendRespData(pCLIReq, "\n</output>\n");

    pCLIReq->RespStatus = OAM_SrvSuccessXMLResp;
    return true;
}

/* Syntax: show istats [gc-/x/y/z] (mod_1[,mod_2,...,mod_n])|*
 */
static bool processIStatCommand(OAM_SrvCLIRequest_t* pCLIReq)
{
    char* str = pCLIReq->ArgStr[0];

    if (!str)
        return false;

    /* Advance until non white space */
    Srv_EatStrSpaces(str);

    /* Match the show */
    if (strncasecmp(str, "show", 4))
        return false;

    str += 4;
    Srv_EatStrSpaces(str);

    /* Match the istats */
    if (strncasecmp(str, "istats", 6))
        return false;

    str += 6;
    Srv_EatStrSpaces(str);

    /* See if command is for particular board */
    if (!strncasecmp(str, "gc-", 3))
    {
        Board_Id_t boardInfo;
        BoardAttr_getId(&boardInfo);
        if (strncasecmp(str, boardInfo.BoardLabel, strlen(boardInfo.BoardLabel)))
        {
            return false;
        }
        str += strlen(boardInfo.BoardLabel);
        Srv_EatStrSpaces(str);
    }

    /* See if we have a wildcard case or a module list */
    if (!strcmp(str, "*"))
    {
        /* Scan over the module table */
        int i;
        for (i = 0; i < LAST_ID; i++)
        {
            if (WorkArea.moduleTable[i].IStatsFn && (app_ShouldRun((App_ModuleId_t)i, Core_GetExecutionContext()) || (APP_NONE_CTX == WorkArea.moduleTable[i].ExecutionContext)))
            {
                app_IStatsFn_t fn = WorkArea.moduleTable[i].IStatsFn;
                (*fn)(pCLIReq);
            }
        }

        Oam_ExecuteAllIstatsCommand(pCLIReq);
    }
    else
    {
        /* Parse module list (into temporary since strtok destroy the string) */
        static char tmp[128];
        char const* delim = ", ";
        char*       pLast;

        Srv_Snprintf(tmp, sizeof(tmp) - 1, "%s", str);
        tmp[sizeof(tmp) - 1] = '\0';

        str                  = strtok_r(tmp, delim, &pLast);
        while (str)
        {
            int i;
            for (i = 0; i < LAST_ID; i++)
            {
                if ((NULL != WorkArea.moduleTable[i].IStatsFn) &&
                    (app_ShouldRun((App_ModuleId_t)i, Core_GetExecutionContext()) || (APP_NONE_CTX == WorkArea.moduleTable[i].ExecutionContext)) &&
                    (0 == strcasecmp(str, App_GetModuleName((App_ModuleId_t)i))))
                {
                    app_IStatsFn_t fn = WorkArea.moduleTable[i].IStatsFn;
                    (*fn)(pCLIReq);
                }
            }

            Oam_ExecuteIstatsCommand(str, pCLIReq);
            str = strtok_r(NULL, delim, &pLast);
        }
    }

    return true;
}

/* Syntax: testop *
 * The purpose of this command is to log an entry in the messagelog,
 * to synchronize the test step with the node debug printouts.
 */
static bool processTestOpCommand(OAM_SrvCLIRequest_t* pCLIReq)
{
    char* str = pCLIReq->ArgStr[0];

    if (!str)
        return false;

    Srv_EatStrSpaces(str);
    if (strncasecmp(str, "testop ", 7))
        return false;
    str += 7;
    Srv_EatStrSpaces(str);

    App_Log(Log::Notice, APP_ID, "TestOp: %s", str);

    pCLIReq->RespStatus = OAM_SrvSuccessResp;
    return true;
}


/*                                                                        */
/* Syntax: show services epg pgw diagnostics apn-management [gc-/x/y/z]      */
/*         show services epg pgw diagnostics gtpb [gc-/x/y/z]                */
/*         show services epg pgw diagnostics radius [gc-/x/y/z]              */
/*        * show services epg pgw diagnostics dhcp [gc-/x/y/z]               */
/*        * show services epg pgw diagnostics pdp  [gc-/x/y/z]               */
/*         show services epg pgw diagnostics gucp [gc-/x/y/z]                */
/*         show services epg pgw diagnostics gccp [gc-/x/y/z]                */
/*         show services epg pgw diagnostics gtp [gc-/x/y/z]                 */
/*         show services epg pgw diagnostics charging [gc-/x/y/z]            */
/*         show services epg pgw diagnostics all [gc-/x/y/z]                 */
/*   * indicates that interface must be PSC interface                   */


static size_t commandLength(char* str)
{
    const char* cmd[] = { "gtpb", "radius", "pdp",
                          "gucp", "gtp",    "charging",   "dhcp", "all", "gccp", "apn-management" };

    for (size_t i = 0; i < Array_Size(cmd); i++)
    {
        size_t len = strlen(cmd[i]);
        if (!strncasecmp(str, cmd[i], len))
        {
            return len;
        }
    }

    return 0;
}

static void processDiagStatCommand(OAM_SrvCLIRequest_t* pCLIReq)
{
    char* str = pCLIReq->ArgStr[0];

    if (!str)
        return;

    /* Advance until non white space */
    Srv_EatStrSpaces(str);

    const size_t len = commandLength(str);

    if (len == 0) // not valid diagnostics command
    {
        return;
    }

    std::string name = std::string(str, len);

    str += len;
    Srv_EatStrSpaces(str);

    /* See if command is for particular board */
    if (!strncasecmp(str, "gc-", 3))
    {
        Board_Id_t boardInfo;
        BoardAttr_getId(&boardInfo);
        if (strncasecmp(str, boardInfo.BoardLabel, strlen(boardInfo.BoardLabel)))
        {
            return;
        }
    }

    if (name == "all")
    {
        /* Scan over the module table */
        int i;
        for (i = 0; i < LAST_ID; i++)
        {
            if (WorkArea.moduleTable[i].IStatsFn)
            {
                app_IStatsFn_t fn = WorkArea.moduleTable[i].IStatsFn;
                App_Log(Log::Debug, APP_ID, "Excuting req (%s) on module id (%d)", pCLIReq->Req, i);
                (*fn)(pCLIReq);
            }
        }

        Oam_ExecuteAllIstatsCommand(pCLIReq);
    }
    else
    {
        for (int i = 0; i < LAST_ID; i++)
        {
            if (WorkArea.moduleTable[i].IStatsFn && strcasecmp(name.c_str(), App_GetModuleName((App_ModuleId_t)i)) == 0)
            {
                app_IStatsFn_t fn = WorkArea.moduleTable[i].IStatsFn;
                (*fn)(pCLIReq);
                break;
            }
        }

        Oam_ExecuteIstatsCommand(name, pCLIReq);
    }
}

static void app_HandleCmdStrRequestCb(OAM_SrvCLIRequest_t* pCLIReq)
{
    pCLIReq->RespStatus = OAM_SrvFailureResp;

    if (pCLIReq->ArgStr[0])
    {
        while (1)
        {
            if (processMallocStatsCommand(pCLIReq))
                break;
            if (processIStatCommand(pCLIReq))
                break;
            if (processTestOpCommand(pCLIReq))
                break;

            break; /* Always break :-) */
        }
    }
    OAM_SrvReturnCLIResponse(pCLIReq);
}

static void app_HandleDiagRequestCb(OAM_SrvCLIRequest_t* pCLIReq)
{
    if (pCLIReq != NULL)
    {
        pCLIReq->RespStatus = OAM_SrvFailureResp;
        App_Log(Log::Debug, APP_ID, "%s:CommandString= %s", __FUNCTION__, pCLIReq->ArgStr[0]);

        if (pCLIReq->ArgStr[0])
        {
            /* Look for an internal stat command */
            processDiagStatCommand(pCLIReq);
        }
        if (pCLIReq->RespStatus != OAM_SrvEagainResp)
        {
            OAM_SrvReturnCLIResponse(pCLIReq);
        }
    }
}

void app_RegisterOamCb()
{
    OAM_SrvRegisterCallback(OAM_SRV_CMD_STR_REQ_ID, app_HandleCmdStrRequestCb);
    OAM_SrvRegisterCallback(OAM_SRV_SHOW_DIAG_REQ_ID, app_HandleDiagRequestCb);
}

#endif  /* FDHCPD */
