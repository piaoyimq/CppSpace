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
#include "app_local.h"
#include "srvlib/include/srvutil.h"
#include "hash_functions.h"
#include "monotonic_time/monotonic_time.h"

#include <string.h>
#include <time.h>

static void App_SetProcessName(const char* procName);
static void App_SetNbPscInstances(const int nbInst);

/*
 * Make an exception here since APP must use these functions to offer its services.
 */
#ifdef APP_ABORT_ERR
#undef abort
#endif
#ifdef APP_EXIT_ERR
#undef exit
#endif
#ifdef APP_SYSLOG_ERR
#undef syslog
#endif

#define MAX_PIAF_PHYSICAL_PROCESS_ID_LEN 256 /* The Physical Process Id used in the PIAF framwork */
#define MAX_PROCESS_NAME_LEN    256
#define MAX_INI_FILE_LEN        256

/**
 * WorkArea
 */
app_CB WorkArea;

/*
 * Direct pointer to moduleTable in WorkArea to be able to inline Logging_getLogMask functions.
 * Kind of breaks the workarea concept but performance is king ...
 */
App_ModuleTableEntryStruct_t* _app_module_table = NULL;

static struct
{
    Ggsn_InstanceId_t instanceId;
    int32_t           nbOfPscInstances;
    char              szProcessName[MAX_PROCESS_NAME_LEN];
    char              szPiafPhysicalProcessId[MAX_PIAF_PHYSICAL_PROCESS_ID_LEN];
    bool              bRunInParentConsole;
    bool              bIsSim;
    bool              bGscAppointedByArg;
    void*             hRedirectedStdout[GGSN_MAX_NB_OF_INSTANCES];
} s_gtpcd = {
    GGSN_INVALID_INSTANCE_ID,
    DEFAULT_NB_PSC_INSTANCE,
    "",
    "",
    true,                    false,                  false,
    { NULL,                  NULL,                   NULL,                  NULL, NULL, NULL, NULL }
};

/* start with -x argument to turn off heartbeat monitoring of instances */
static bool                         instanceGdbDebugFlag = false;

static App_ModuleTableEntryStruct_t app_initModuleTable  =
{ APP_ID, "APP", NULL, NULL, NULL, Log::AllMask, APP_NONE_CTX, 0 };

/* Init and Shutdown */
/* ----------------- */

static void App_SetHaltCb(App_AbortFn_t AbortFn)
{
    WorkArea.AbortFn = AbortFn;
}

static void App_SetExitCb(App_ExitFn_t ExitFn)
{
    WorkArea.ExitFn = ExitFn;
}

static void App_SetModuleTable(const App_ModuleTableEntryStruct_t table[], const size_t tableSize, const bool debugLogLevelOverideFlag)
{
    APP_ASSERT_MSG(LAST_ID == tableSize, APP_ID, "%s: Invalid tableSize= %zu(must be: %d)",
            __FUNCTION__, tableSize, LAST_ID);

    memset(WorkArea.moduleTable, 0, sizeof(WorkArea.moduleTable));

    for (int i = 0; i < LAST_ID; i++)
    {
        WorkArea.moduleTable[table[i].ModuleId] = table[i];
        const Log::Mask_t logMask = debugLogLevelOverideFlag ?
                                    Log::DebugMask : table[i].LogMask;
        Logging_setLogMask(table[i].ModuleId, logMask, table[i].ModuleName, table[i].LogMemorySize);
    }

    for (int i = 0; i < LAST_ID; i++)
    {
        TRACE(Log::Debug, APP_ID, "%s: getting default log level for (%s) id:(%d) mask:(0x%X)",
                __FUNCTION__, table[i].ModuleName, table[i].ModuleId, table[i].LogMask);
    }
}

void App_Init(const char* processName, const int numOfInstances, const Ggsn_InstanceId_t instanceId,
        const App_ModuleTableEntryStruct_t table[], const size_t tableSize, const bool debugLogLevelOverideFlag,
        App_AbortFn_t abortFn, App_ExitFn_t exitFn)
{
    App_SetHaltCb(abortFn);
    App_SetExitCb(exitFn);

    WorkArea.AbortMutex = new srvlib::Mutex(srvlib::Mutex::Recursive);

    memset(WorkArea.moduleTable, 0, sizeof(WorkArea.moduleTable));

    WorkArea.moduleTable[APP_ID] = app_initModuleTable;

    _app_module_table            = WorkArea.moduleTable;

    WorkArea.ExecutionContext    = APP_NONE_CTX;
    WorkArea.HighestContext      = APP_NONE_CTX;

    WorkArea.pSymbolTable        = SymbolTable_Create();

#ifndef FDHCPD
    app_RegisterOamCb();
#endif

    App_SetProcessName(processName);
    App_SetInstanceId(instanceId);
    App_SetNbPscInstances(numOfInstances);
    App_SetModuleTable(table, tableSize, debugLogLevelOverideFlag);
}

void App_Shutdown()
{
    /* Don't delete stray symbols so that purify may identify them. */
    SymbolTable_Destroy(WorkArea.pSymbolTable, false);

    /* Note: The AbortMutex can't be destroyed here because it's held when this function is called. */
}

/* Module entry handling */
/* --------------------- */

Core_InitCallbackFn_t App_GetInitFn(App_ModuleId_t ModuleId)
{
    return WorkArea.moduleTable[ModuleId].InitFn;
}

Core_ShutdownCallbackFn_t App_GetShutdownFn(App_ModuleId_t ModuleId)
{
    return WorkArea.moduleTable[ModuleId].ShutdownFn;
}

/* Critical functions handling */
/* --------------------------- */

static unsigned int applicationTerminating = 0;

void App_Halt(App_ModuleId_t ModuleId, const char* FileName, int32_t LineNb)
{
    /* Note: The mutex is recursive so the same thread can enter many times.
     *       Other threads will get blocked on the mutex.
     */
    if (WorkArea.AbortMutex != NULL)
    {
        WorkArea.AbortMutex->lock();
    }

    if (applicationTerminating++ != 0)
    {
        App_Log(Log::Critical, ModuleId, "%s: Recursive call to abort (applicationTerminating=%d), "
                "terminating right away", __FUNCTION__, applicationTerminating);
        abort();
    }

    if (WorkArea.AbortFn)
    {
        (WorkArea.AbortFn)(ModuleId, FileName, LineNb);
        /* Call abort() in all paths to make the compiler happy as this function is marked as "noreturn". */
        abort();
    }

    App_Log(Log::Critical, ModuleId, "Aborting (filename=%s, line#=%d)", FileName, LineNb);
    abort();
}


void App_Exit(App_ModuleId_t ModuleId, const char* FileName, int32_t LineNb, int ExitCode, App_ExitMsg_t ExitMsg)
{
    /* Note: The mutex is recursive so the same thread can enter many times.
     *       Other threads will get blocked on the mutex.
     */
    if (WorkArea.AbortMutex != NULL)
    {
        WorkArea.AbortMutex->lock();
    }

    if (applicationTerminating++ != 0)
    {
        App_Log(Log::Critical, ModuleId, "%s: Recursive call to exit (applicationTerminating=%d), "
                "terminating right away", __FUNCTION__, applicationTerminating);
        exit(ExitCode);
    }

    if (WorkArea.ExitFn)
    {
        (WorkArea.ExitFn)(ModuleId, FileName, LineNb, ExitCode, ExitMsg);
        /* Call exit() in all paths to make the compiler happy as this function is marked as "noreturn". */
        exit(ExitCode);
    }

    App_Log(Log::Critical, ModuleId, "Exiting (filename=%s, line#=%d)", FileName, LineNb);
    exit(ExitCode);
}

void App_TrivialExit(App_ModuleId_t ModuleId, const char* FileName, int32_t LineNb)
{
    App_Log(Log::Notice, ModuleId, "Exiting trivially (filename=%s, line#=%d)", FileName, LineNb);
    exit(0);
}

void App_setInstanceGdbDebugFlag(const bool debug)
{
    instanceGdbDebugFlag = debug;
}

bool App_getInstanceGdbDebugFlag()
{
    return instanceGdbDebugFlag;
}

/* Execution context handling */
/* -------------------------- */

static bool ShouldRun(App_ModuleId_t moduleId, App_ExecutionContext_t currentContextId)
{
    const uint32_t moduleContextAndFlags = WorkArea.moduleTable[moduleId].ExecutionContext; /* The full condition (contexts and flags) for this module */
    bool           shouldRun             = currentContextId & moduleContextAndFlags;        /* The main handling, i.e. shall this module run in this context? */

    if (shouldRun)
    {
        switch (currentContextId)
        {
            case APP_BASIC_CTX:
            #define BASIC_INSTANCE_FLAG (APP_BASIC_CTX_IF_INSTANCE & ~APP_BASIC_CTX)
            #define BASIC_MAIN_FLAG (APP_BASIC_CTX_IF_MAIN & ~APP_BASIC_CTX)
                if (moduleContextAndFlags & (BASIC_INSTANCE_FLAG | BASIC_MAIN_FLAG)) /* Shall we look at the instance parameters? */
                {
                    /* Adjust accordingly */
                    shouldRun = IS_INSTANCE() ? (moduleContextAndFlags & BASIC_INSTANCE_FLAG) : (moduleContextAndFlags & BASIC_MAIN_FLAG);
                }
                break;
            case APP_PSC_CTX:
            #define PSC_INSTANCE_FLAG (APP_PSC_CTX_IF_INSTANCE & ~APP_PSC_CTX)
            #define PSCMAIN_FLAG     (APP_PSC_CTX_IF_MAIN & ~APP_PSC_CTX)
                if (moduleContextAndFlags & (PSC_INSTANCE_FLAG | PSCMAIN_FLAG)) /* Shall we look at the instance parameters? */
                {
                    /* Adjust accordingly */
                    shouldRun = IS_INSTANCE() ? (moduleContextAndFlags & PSC_INSTANCE_FLAG) : (moduleContextAndFlags & PSCMAIN_FLAG);
                }
                break;
            case APP_GSC_CTX:
                /* Ignore the extra flags if any */
                break;
            default:
                App_Log(Log::Critical, APP_ID, "%s: Unexpeced currentContextId=%u", __FUNCTION__, currentContextId);
                APP_ABORT(APP_ID);
                break;
        }
    }

    return shouldRun;
}

uint32_t App_GetHighestContext()
{
    return WorkArea.HighestContext;
}

int32_t App_AddExecutionContext(App_ExecutionContext_t ContextId,
        App_ModuleId_t* aModulesToStart,
        int32_t* NbModulesToStart)
{
    uint32_t Count = 0, i, Error = 0;

    if (WorkArea.ExecutionContext & ContextId)
    {
        App_Log(Log::Error, APP_ID, "Context %s already set",
                App_ExecutionContextName(ContextId));
        (*NbModulesToStart) = 0;
        return -1;
    }
    WorkArea.ExecutionContext |= ContextId;

    for (i = 0; i < LAST_ID; i++)
    {
        /* That module runs in the context being started and
           it has an init function */
        if (ShouldRun((App_ModuleId_t)i, ContextId) && WorkArea.moduleTable[i].InitFn)
        {
            if ((int32_t)Count >= (*NbModulesToStart))
            {
                /* The aModulesToStart table is too small */
                Error = 1;
            }
            else
            {
                App_Log(Log::Debug, APP_ID, "%s: Start %s ", __FUNCTION__, App_GetModuleName((App_ModuleId_t)i));
                aModulesToStart[Count] = WorkArea.moduleTable[i].ModuleId;
            }
            Count++;
        }
    }
    if (Error)
    {
        /* Roll back the execution context addition */
        WorkArea.ExecutionContext &= ~ContextId;

        (*NbModulesToStart)        = 0;
        return Count;
    }

    WorkArea.HighestContext = ContextId;    /* latch the highest execution context attained */
    (*NbModulesToStart)     = Count;
    return 0;
}


int32_t App_RemoveExecutionContext(App_ExecutionContext_t ContextId,
        App_ModuleId_t* aModulesToStop, int32_t* NbModulesToStop,
        App_ModuleId_t* aModulesToKill, int32_t* NbModulesToKill)
{
    uint32_t Count = 0, i, Error = 0;

    if (!(WorkArea.ExecutionContext & ContextId))
    {
        App_Log(Log::Error, APP_ID, "Context %s already removed",
                App_ExecutionContextName(ContextId));
        (*NbModulesToStop) = 0;
        (*NbModulesToKill) = 0;
        return -1;
    }

    WorkArea.ExecutionContext &= ~ContextId;

    /* Fill up the table of modules that shutdown function has to be called
       for this phase */
    for (i = 0; i < LAST_ID; i++)
    {
        /* The module was running in the context being shutdown
             and it has a shutdown function */
        if (ShouldRun((App_ModuleId_t)i, ContextId) && WorkArea.moduleTable[i].ShutdownFn)
        {
            if ((int32_t) Count >= (*NbModulesToStop))
            {
                /* The aModulesToStop table is too small */
                Error = 1;
            }
            else
            {
                aModulesToStop[Count] = WorkArea.moduleTable[i].ModuleId;
            }
            Count++;
        }
    }
    if (Error)
    {
        (*NbModulesToStop) = 0;
        (*NbModulesToKill) = 0;

        /* Roll back the exectuion context removal */
        WorkArea.ExecutionContext |= ContextId;
        return Count;
    }

    (*NbModulesToStop) = Count;

    Count              = 0;

    /* Fill up the table of module that exectuion is terminated in this phase */
    for (i = 0; i < LAST_ID; i++)
    {
        /* The module is running in the context being shut down AND after
           this context is shutdown it has no more exectuion contexts remaining,
           it does not matter if the module has or not a shutdown function */
        if ((!(WorkArea.moduleTable[i].ExecutionContext & WorkArea.ExecutionContext)) &&
            (WorkArea.moduleTable[i].ExecutionContext & ContextId))
        {
            if ((int32_t) Count >= (*NbModulesToKill))
            {
                /* The aModulesToKill table is too small */
                Error = 1;
            }
            else
            {
                aModulesToKill[Count] = WorkArea.moduleTable[i].ModuleId;
            }
            Count++;
        }
    }

    if (Error)
    {
        (*NbModulesToStop) = 0;
        (*NbModulesToKill) = 0;

        /* Roll back the exectuion context removal */
        WorkArea.ExecutionContext |= ContextId;
        return Count;
    }

    (*NbModulesToKill) = Count;

    return 0;
}




/* Utility functions */
/* ----------------- */

const char* App_ExecutionContextName(App_ExecutionContext_t ContextId)
{
    switch (ContextId)
    {
        case APP_NONE_CTX:
            return "NONE";
        case APP_BASIC_CTX:
            return "BASIC";
        case APP_PSC_CTX:
            return "PSC";
        case APP_GSC_CTX:
            return "GSC";
        case APP_ALL_CTX:
            return "ALL";
        case APP_BASIC_CTX_IF_INSTANCE:
            return "BASIC(inst)";
        case APP_BASIC_CTX_IF_MAIN:
            return "BASIC(main)";
        case APP_PSC_CTX_IF_INSTANCE:
            return "PSCi";
        case APP_PSC_CTX_IF_MAIN:
            return "PSCm";
    }
    return "UNKNOWN";
}

bool app_ShouldRun(App_ModuleId_t moduleId, App_ExecutionContext_t currentContextId)
{
    return ShouldRun(moduleId, currentContextId);
}

/* Application wide symbol table */
/* ----------------------------- */

Symbol_t* App_CreateSymbolFromCstr(const char* pStr)
{
    return SymbolTable_CreateSymbolFromCstr(WorkArea.pSymbolTable, pStr);
}

Symbol_t* App_CreateSymbolFromString(const std::string& string)
{
    return SymbolTable_CreateSymbolFromString(WorkArea.pSymbolTable, string);
}

Symbol_t* App_CreateSymbol(const uint8_t* pData, uint32_t length)
{
    return SymbolTable_CreateSymbolFromData(WorkArea.pSymbolTable, (const uint8_t*)pData, length);
}

Symbol_t* App_Symbol_Unpack(ITlvDecoder& dec) { return Symbol_Unpack(WorkArea.pSymbolTable, dec); }

/****************************************************************************/

static void App_SetNbPscInstances(const int nbInst)
{
    APP_ASSERT(nbInst > 0);
    APP_ASSERT(nbInst <= GGSN_MAX_NB_OF_INSTANCES);
    s_gtpcd.nbOfPscInstances = nbInst;
}

int App_GetNbPscInstances()
{
    APP_ASSERT(s_gtpcd.nbOfPscInstances > 0 && s_gtpcd.nbOfPscInstances <= GGSN_MAX_NB_OF_INSTANCES);
    return s_gtpcd.nbOfPscInstances;
}

bool App_IsValidInstanceId(const Ggsn_InstanceId_t instId)
{
    return instId < App_GetNbPscInstances();
}

Ggsn_InstanceId_t App_GetInstanceId()
{
    return s_gtpcd.instanceId;
}

void App_SetInstanceId(const Ggsn_InstanceId_t instId)
{
    APP_ASSERT(instId < GGSN_MAX_NB_OF_INSTANCES || instId == GGSN_INVALID_INSTANCE_ID);
    s_gtpcd.instanceId = instId;
}

const char* App_GetProcessName()
{
    APP_ASSERT(*s_gtpcd.szProcessName);
    return s_gtpcd.szProcessName;
}

static void App_SetProcessName(const char* procName)
{
    Srv_Strncpy(s_gtpcd.szProcessName, procName, sizeof(s_gtpcd.szProcessName));
    s_gtpcd.szProcessName[sizeof(s_gtpcd.szProcessName) - 1] = '\0';
}

bool App_GetRunInParentConsole()
{
    return s_gtpcd.bRunInParentConsole;
}

void App_SetRunInParentConsole(bool bRunInParentConsole)
{
    s_gtpcd.bRunInParentConsole = bRunInParentConsole;
}

void App_ParseRedirectHandles(char* handles)
{
    char* pToken = NULL;
    char* pLast  = NULL;
    int instance = 0;

    pToken = strtok_r(handles, ",", &pLast);
    while (pToken && (instance < GGSN_MAX_NB_OF_INSTANCES))
    {
        s_gtpcd.hRedirectedStdout[instance] = (void*)(uintptr_t)atoi(pToken);
        instance++;

        pToken = strtok_r(NULL, ",", &pLast);
    }
}

void* App_GetRedirectHandle(int instance)
{
    APP_ASSERT((instance < GGSN_MAX_NB_OF_INSTANCES));
    return s_gtpcd.hRedirectedStdout[instance];
}

void App_SetPiafPhysicalProcessId(const char* ppid)
{
    Srv_Strncpy(s_gtpcd.szPiafPhysicalProcessId, ppid, sizeof(s_gtpcd.szPiafPhysicalProcessId));
}

const char* App_GetPiafPhysicalProcessId()
{
    return s_gtpcd.szPiafPhysicalProcessId;
}

void App_SetSim(bool isSim)
{
    s_gtpcd.bIsSim = isSim;
}

bool App_IsSim()
{
    return s_gtpcd.bIsSim;
}

/* GSC_DEPLOYED_BY_DCM */
bool App_IsGscAppointedByArg()
{
    return s_gtpcd.bGscAppointedByArg;
}

void App_SetGscAppointedByArg(bool bGscAppointedByArg)
{
    s_gtpcd.bGscAppointedByArg = bGscAppointedByArg;
}

/* ------------------------------------------------------------------------- */
const char* App_GetModuleName(const App_ModuleId_t ModuleId)
{
    /* if we crash here, it mean that some new module is not added in gtpcd.cc::gtpcd_InitTable[]
     * or that we are calling this function "to early" - make sure the table is initialized
     * TODO: QPETKAN: Initialize this table first thing in main() */
    APP_ASSERT(((uint32_t)ModuleId < LAST_ID) && (WorkArea.moduleTable[ModuleId].ModuleName != NULL));
    return WorkArea.moduleTable[ModuleId].ModuleName;
}

/* ------------------------------------------------------------------------- */
