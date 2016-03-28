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

#define TRACEPOINT_CREATE_PROBES
#define TRACEPOINT_DEFINE
#include "app_lttng_debuglog_provider.h"

#include <stdio.h>
#include <stdarg.h>

void Logging_logLttng(unsigned short loglevel, const char* modname, const char* format, ...)
{
    char    log_message[500];
    va_list arglist;
    va_start(arglist, format);
    /* could use vasprintf for dynamic array sizes, but its likely faster to allocate it on
     * on the stack like this (and having larger strings than 500 chars seems unlikely   */
    vsnprintf(log_message, sizeof(log_message), format, arglist);
    va_end(arglist);

    /* since we already checked that the tracepoint is enabled in the App_LogLttng macro. we call the tracepoint cb
     * directly, thus avoiding doing a dual check */
    __tracepoint_cb_ericsson_epg_pgw_applog___debuglog(loglevel, modname, log_message);
}
