#ifndef APP_INCLUDE_APP_GLOBAL_H_
#define APP_INCLUDE_APP_GLOBAL_H_
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
#include "app/include/module_id.h"
#include <stdint.h>

/* --- Application specific definitions --- */

/* Module names */
/* ------------ */

/* Now initialized in app.c */
#define APP_MAX_NAME_SIZE          (8)

/* Execution context */
/* ----------------- */

enum App_ExecutionContext_t
{
    APP_NONE_CTX              = 0x00, /* No execution context       */
    APP_BASIC_CTX             = 0x01, /* Basic execution context */
    APP_PSC_CTX               = 0x02, /* PSC execution context */
    APP_GSC_CTX               = 0x04, /* GSC execution context */
    APP_ALL_CTX               = APP_BASIC_CTX  |  APP_PSC_CTX  | APP_GSC_CTX,
    /* The following constants are used when a start phase shall run in only one of Main and Instance
     * Note that the combination "APP_GSC_CTX | APP_BASIC_CTX_IF_INSTANCE" is not allowed
     * (i.e we do not want to run in basic if PSCi) since we cannot discriminate between
     * PSCm and GSC in Basic context */
    APP_BASIC_CTX_IF_INSTANCE = APP_BASIC_CTX | 0x0010, /* Basic execution context if PSCi */
    APP_BASIC_CTX_IF_MAIN     = APP_BASIC_CTX | 0x0020, /* Basic execution context if PSCm */
    APP_PSC_CTX_IF_INSTANCE   = APP_PSC_CTX | 0x0040,   /* PSC execution context if PSCi */
    APP_PSC_CTX_IF_MAIN       = APP_PSC_CTX | 0x0080    /* PSC execution context if PSCm */
/*  APP_GSC_CTX_IF_INSTANCE= APP_GSC_CTX| 0x0100, --- Not used until the GSC is "instantiated" */
/*  APP_GSC_CTX_IF_MAIN    = APP_GSC_CTX| 0x0200  --- Not used until the GSC is "instantiated" */
};

/* official terminology */
#define GSC_NAME           "Global Session Controller"
#define PSC_NAME           "PGW Session Controller"
#define PGW_PPB_NAME       "PGW Packet Processor"
#define SGW_PPB_NAME       "SGW Packet Processor"
#define SC_NAME            "Session Controller"
#define PPB_NAME           "Packet Processor"
#define PPBT_NAME          "L2TP Packet Processor"
#define BACKUP_BOARD_NAME  "Secondary PGW Session Controller"    /* Role only exists if Session Resilience is enabled. CPB assigned to a primary PSC*/
#define SPARE_BOARD_NAME   "Spare"

/* --- Protocol related general defines --- */


#endif /* APP_INCLUDE_APP_GLOBAL_H_ */
