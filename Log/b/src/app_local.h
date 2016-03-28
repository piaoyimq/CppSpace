#ifndef APP_SRC_APP_LOCAL_H_
#define APP_SRC_APP_LOCAL_H_
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

/******************* includes ****************/

#include "app/include/app_global.h"
#include "app/include/app.h"
#include "srvlib/include/mutex.h"
#include <stdint.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdarg.h>

struct app_CB
{
    /* Mutex needed to solve problem when two threads abort at once. */
    srvlib::Mutex* AbortMutex;

    /* Critical error handling functions */
    /* --------------------------------- */
    App_AbortFn_t AbortFn;      /* abort function */
    App_ExitFn_t  ExitFn;       /* exit function */

    /* Module table */
    /* ------------ */
    App_ModuleTableEntryStruct_t moduleTable[LAST_ID];         /* Copy of GTPCD module table, updated when log level change. */

    /* Execution context */
    /* ----------------- */
    uint32_t       ExecutionContext;      /* Current execution context */
    uint32_t       HighestContext;        /* Highest execution context reached, i.e., GSC > PSC > Basic */

    SymbolTable_t* pSymbolTable;   /* Singleton instance of symbol table. */
};

extern app_CB WorkArea;

/* Local functions */

void app_RegisterOamCb();

#endif /* APP_SRC_APP_LOCAL_H_ */
