/* ----------------------------------------------------------------------
 * %CCaseRev: %
 * %CCaseDate: %
 * ProductNr:           m/190 32-CAA xxx nn
 * Author: Tamanna Islam, lmctais@lmc.ericsson.se
 *
 * Short description:
 * This is the main application module, it offers the following services:
 * - Critical function handling
 * - Logger functions
 * - Module Ids and Names
 * - Exectution context management
 *
 * Important note:
 * --------------
 * The default logger output is the syslog, it is possible to redirect it
 * toward an opened file descritor. The redeirection should be performed
 * for the basic test purpose only.
 * ----------------------------------------------------------------------
 * Template Id: LMC/XD-00:003 Rev PA1
 *
 * Copyright (C) 2000 by Ericsson Telecom AB
 * S - 125 26  STOCKHOLM
 * SWEDEN, tel int + 46 8 719 0000
 *
 * This program may be used and/or copied only with the written permission
 * from Ericsson Telecom AB, or in accordance with the terms and
 * conditions stipulated in the agreement/contract under which the program
 * has been supplied.
 *
 * All rights reserved.
 *
 * ----------------------------------------------------------------------
 *
 * !!! Revision history shall be documented using the Clearcase checkin
 * comments!!!!
 * ----------------------------------------------------------------------
 */
/****** THIS LINE IS 80 CHARACTERS WIDE - DO *NOT* EXCEED 80 CHARACTERS! ******/

#ifndef _APP_H
#define _APP_H

/******************* includes ****************/
#include "app/Logging/include/logging.h"
#include "core/include/core_phase.h"
#include "srvlib/include/symbol.h"
#include "ggsn.h"

/* Forward declarations */
struct OAM_SrvCLIRequest_t;

/*************** defines ***************/

/* Module data */
/* ----------- */
/*
 * Hint to APP_EXIT and callbacks what kind of exit message that could be presented.
 * It's up to the callback how to interpret this information or to ignore it entirely.
 */
enum App_ExitMsg_t
{
    APP_EXIT_MSG_NORMAL,
    APP_EXIT_MSG_ERROR,
};

/*
 * Prototypes of the abort and exit functions
 * These functions should never return!
 */
typedef void (* App_AbortFn_t) (App_ModuleId_t ModuleId, const char* FileName, int32_t LineNb);
typedef void (* App_ExitFn_t)  (App_ModuleId_t ModuleId, const char* FileName, int32_t LineNb, int ExitCode, App_ExitMsg_t ExitMsg);

typedef void (* app_IStatsFn_t)(OAM_SrvCLIRequest_t*);

/* Structure containing module data */
struct App_ModuleTableEntryStruct_t
{
    App_ModuleId_t            ModuleId;           /* Assigned module Id */
    const char*               ModuleName;         /* Module name */
    Core_InitCallbackFn_t     InitFn;             /* Pointer to the module init function */
    Core_ShutdownCallbackFn_t ShutdownFn;         /* Pointer to the module shutdown function */
    app_IStatsFn_t            IStatsFn;           /* Function for getting internal module statistics */
    Log::Mask_t               LogMask;            /* Module log mask */
    int                       LogMemorySize;      /* How big this internal memory log buffer is in bytes */
    uint32_t                  ExecutionContext;   /* Module exectuion contextes */
    /* Notes:
       - The log mask consists in a set of ORed flags indicating the circumstances
         in which the module will produce logs
       - Exectuion context inicdates in which contexts the module is active, when
         this field is set to APP_NONE_CTX the module will never be invoked
         for a shutdown init phase, its postopend operations will always remain */
};

/*
 * To be able to make Logging_getLogMask an inline function (showed up as 1% of total GTPCD cost).
 * Don't use this pointer directly. Although it would be safe, just don't do it.
 */
extern App_ModuleTableEntryStruct_t* _app_module_table;


/* Init functions */
/* -------------- */

/******************************************************************************
   Function:    App_Init

   Description: Initializes the App module global variables, this function should
             be called once in the application.

   Return:      None.

   Output:      None.

   Modified:    None.

   Input:       None.

******************************************************************************/
void App_Init(const char* processName, const int numOfInstances, const Ggsn_InstanceId_t instanceId,
        const App_ModuleTableEntryStruct_t table[], const size_t tableSize, const bool debugLogLevelOverideFlag,
        App_AbortFn_t abortFn, App_ExitFn_t exitFn);
void App_Shutdown();


/* Module entry handling */
/* --------------------- */

/******************************************************************************
   Function:    App_GetInitFn

   Description: Returns the pointer to a module Init function. Certain modules do
             not have any Init function, a NULL value is returned in that case.

   Return:      Non NULL on success when a module has an Init funtions
             NULL otherwise.

   Output:      None.

   Modified:    None.

   Input:       Index: Index in the module table.

******************************************************************************/
Core_InitCallbackFn_t App_GetInitFn(App_ModuleId_t);


/******************************************************************************
   Function:   App_GetShutdownFn

   Description: Returns the pointer to a module Shutdown function. Certain modules
             do not have any Shutdown function, a NULL will be returned in that
             case.

   Return:      Non NULL on success when a module has an Shutdown funtions
             NULL otherwise.

   Output:      None.

   Modified:    None.

   Input:       Index: Index in the module table.

******************************************************************************/
Core_ShutdownCallbackFn_t App_GetShutdownFn(App_ModuleId_t);


/******************************************************************************
   Function:    App_GetModuleName

   Description: Retruns the name of the module identified by ModuleId.

   Return:      Module name string.

   Output:      None.

   Modified:    None.

   Input:       ModuleId: Module Id.

******************************************************************************/
const char* App_GetModuleName(const App_ModuleId_t ModuleId);

void App_setInstanceGdbDebugFlag(const bool debug);

bool App_getInstanceGdbDebugFlag();


/* Critical functions handling */
/* --------------------------- */

#if defined(NONORETURNATTR)
/* The noreturn attribute can prevent GDB from performing backtrace properly on JunOS (mips) */
#define attribute_noreturn
#else
#define attribute_noreturn __attribute__ ((noreturn))
#endif

/*
 * Halt the application.
 */
void App_Halt(App_ModuleId_t ModuleId, const char* FileName, int32_t LineNb) attribute_noreturn;

/*
 * Exits the application.
 */
void App_Exit(App_ModuleId_t ModuleId, const char* FileName, int32_t LineNb, int ExitCode, App_ExitMsg_t ExitMsg) attribute_noreturn;

/** Calls exit(0). Needed since App_Exit() logs an emergency...
 *
 * @param ModuleId
 * @param FileName
 * @param int32_t LineNb
 */
void App_TrivialExit(App_ModuleId_t ModuleId, const char* FileName, int32_t LineNb) attribute_noreturn;

#undef attribute_noreturn

/******************************************************************************
   Function:    App_GetHighest

   Description: Returns highest execution context attained to date
             (GSC > PSC > Basic )

   Return:      Current highest executiion context.

   Output:      None.

   Modified:

   Input:

******************************************************************************/
uint32_t App_GetHighestContext();


/******************************************************************************
   Function:    App_AddExecutionContext

   Description: Adds a context to the current execution context. The user
             must provide a table that will be filled with the IDs of the
             modules that init function has to be called.

   Return:      0 on success
             -1 on failure due to an attempt to add a wrong context
             (e.g. already added).
             >0 the table is not large enough and needs to be increased
             to the indicated size.
             In all the error cases the exectuion context is not updated.

   Output:      None.

   Modified:    aModulesToStart: Table contains a list of module IDs of all the
             modules the init function has to be invoked.
             NbModulesToStart: Number of module that are contained in the
             aModulesToStart table.

   Input:       App_ExecutionContext_t (ContextId): Context to add (being started).
             aModulesToStart: Table that will be filled up with the module IDs
             NbModulesToStart: Available space in the aModulesToStart table.

******************************************************************************/
int32_t App_AddExecutionContext(App_ExecutionContext_t ContextId,
        App_ModuleId_t* aModulesToStart,
        int32_t* NbModulesToStart);


/******************************************************************************
   Function:    App_RemoveExecutionContext

   Description: Removes a context from the current execution context. The user
             must provide a table that will be filled with the IDs of the
             modules that shutdown function has to be called. The user must also
             privide a table that will be filled with the IDs of modules that
             are definitively stopped (the context being shut down is the
             last context these modules run in).

   Return:      0 on success
             -1 on failure due to an attempt to remove a wrong context
             (e.g. already removed).
             >0 one of the tables is not large enough and needs to be increased
             to the indicated size.
             In all the error cases the exectuion context is not updated.

   Output:      None.

   Modified:    aModulesToStop: Table contains a list of module IDs of all the
             modules the shutdown function has to be invoked.
             NbModulesToStop: Number of module that are contained in the
             aModulesToStop table.
             aModulesToKill: Table contains a list of module Ids of all the
             modules that the last exectuion context is being shut down.

   Input:       App_ExecutionContext_t (ContextId): Context to remove (being
             shut down).
             aModulesToStop: Table that will be filled up with the module IDs
             NbModulesToStop: Available space in the aModulesToStop table.
             aModulesToKill: Table that will be filled up with the module IDs
             NbModulesToKill: Available space in the aModulesToKill table.

******************************************************************************/
int32_t App_RemoveExecutionContext(App_ExecutionContext_t ContextId,
        App_ModuleId_t* aModulesToStop, int32_t* NbModulesToStop,
        App_ModuleId_t* aModulesToKill, int32_t* NbModulesToKill);



/* Utility functions */
/* ----------------- */

/******************************************************************************
   Function:    App_ExecutionContextName

   Description: Returns the execution context name from the execution context id.

   Return:      None.

   Output:      None.

   Modified:    None.

   Input:       None.

******************************************************************************/
const char* App_ExecutionContextName(App_ExecutionContext_t ContextId);


/******************************************************************************
   Function:    app_ShouldRun

   Description: Tells whether a particular Module should run or not depending on the current
             execution context and Instance'ness of a gtpcd process.

   Return:      true if the module 'moduleId' should run in the current execution context 'contextId'.
             otherwise false

   Output:      None.

   Modified:    None.

   Input:       moduleId   The module Id.
             contextId  Current execution context of the process.

******************************************************************************/
bool app_ShouldRun(App_ModuleId_t moduleId, App_ExecutionContext_t currentContextId);

/**
 * Create symbol through singleton instance of symbol table (C string version).
 * @see symbol.h
 */
Symbol_t* App_CreateSymbolFromCstr(const char* pStr);

/**
 * Create symbol through singleton instance of symbol table (std::string version).
 * @see symbol.h
 */
Symbol_t* App_CreateSymbolFromString(const std::string& string);

/**
 * Create symbol through singleton instance of symbol table.
 * @see symbol.h
 */
Symbol_t* App_CreateSymbol(const uint8_t* pData, uint32_t length);

/**
 *  Serialization of Symbols
 */
static inline void App_Symbol_Pack(const Symbol_t* pSymbol, ITlvEncoder& enc) { Symbol_Pack(pSymbol, enc); }

Symbol_t* App_Symbol_Unpack(ITlvDecoder& dec);

/* Macros */
/* ------ */
// APP_ASSERT is always on, even in customer builds
#define APP_ASSERT_MSG(expr, moduleId, ...)                             \
    do {                                                                \
        if (!(expr))                                                    \
        {                                                               \
            if (PP_NARG(__VA_ARGS__) > 0)                               \
            {                                                           \
                Logging_log(Log::Emergency, moduleId, ## __VA_ARGS__);       \
            }                                                           \
            APP_ABORT(moduleId, __VA_ARGS__);                           \
        }                                                               \
    } while (0)

#define APP_ASSERT(expr)                                                \
    do {                                                                \
        if (!(expr))                                                    \
        {                                                               \
            APP_ABORT(APP_ID);                                          \
        }                                                               \
    } while (0)

// APP_ASSERT_DEBUG is similar to APP_ASSERT, but is disabled in customer builds
#ifdef APP_ASSERT_DEBUG_OFF
#define APP_ASSERT_DEBUG(expr)
#else
#define APP_ASSERT_DEBUG(expr)                                          \
    do {                                                                \
        if (!(expr))                                                    \
        {                                                               \
            APP_ABORT(APP_ID);                                          \
        }                                                               \
    } while (0)
#endif

#define APP_ABORT(ModuleId, ...) \
    { \
        App_Halt(ModuleId, __FILE__, __LINE__); \
    }

#define APP_EXIT(ModuleId, ExitMsg) \
    { \
        App_Exit(ModuleId, __FILE__, __LINE__, 0, ExitMsg); \
    }

#define APP_TRIVIAL_EXIT(ModuleId) \
    { \
        App_TrivialExit(ModuleId, __FILE__, __LINE__); \
    }

/* ----------------------------------------------------------------------------------------------------------------- */

#define IS_INSTANCE() (App_GetInstanceId() != GGSN_INVALID_INSTANCE_ID)   /* Returns true if called from instance "mode" */
#define IS_PSCMAIN()  (Core_GetExecutionContext() == APP_PSC_CTX && !IS_INSTANCE())


/** Macro to be used to find out whether the application is compiled to be fully deployed from DCM.
 * @return true if compiled for full DCM deployment, false otherwise.
 */
#if defined (CONFIG_GSC_DEPLOYED_BY_DCM)
#define APP_IS_DCM_DEPLOYED() (true)
#else
#define APP_IS_DCM_DEPLOYED() (false)
#endif

#if defined (CONFIG_PGWC_SINGLEIP)
#define APP_IS_PGWC_SINGLEIP() (true)
#else
#define APP_IS_PGWC_SINGLEIP() (false)
#endif


#define APP_BOARDID_LEN 5
#define APP_INSTANCE_ID_LEN 4

/* Help makros */
#define APP_CREATE_MASK(len)          ((1 << (len)) - 1)

int App_GetNbPscInstances();
Ggsn_InstanceId_t App_GetInstanceId();
void App_SetInstanceId(const Ggsn_InstanceId_t instId);
bool App_IsValidInstanceId(const Ggsn_InstanceId_t instId);

const char* App_GetProcessName();

bool App_GetRunInParentConsole();
void App_SetRunInParentConsole(bool bRunInParentConsole);

void App_ParseRedirectHandles(char* handles);
void* App_GetRedirectHandle(int instance);

void App_SetPiafPhysicalProcessId(const char* ppid);
const char* App_GetPiafPhysicalProcessId();


void App_SetSim(bool isSim);

/* returns true iff gtpcd is running in simulated environment (ELC or SSR SIM)*/
bool App_IsSim();

bool App_IsGscAppointedByArg();
void App_SetGscAppointedByArg(bool bGscAppointedByArg);

#endif /* _APP_H */
