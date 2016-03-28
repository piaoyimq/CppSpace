/*---------------------------------------------------------------------------------------------------------------------
* Template Id: 20041119
*
* Copyright (C) 2011 by Ericsson AB
* S - 125 26  STOCKHOLM
* SWEDEN, tel int + 46 8 719 0000
*
* The copyright to the computer program herein is the property of Ericsson AB. The program may be used and/or copied
* only with the written permission from Ericsson AB, or in accordance with the terms andconditions stipulated in the
* agreement/contract under which the program has been supplied.
*
* All rights reserved.
*
*************************    THIS LINE IS 120 CHARACTERS WIDE - DO *NOT* EXCEED 120 CHARACTERS!    ********************
* ---------------------------------------------------------------------------------------------------------------------
*
* Short description: ELC implementation
*
*
* -------------------------------------------------------------------------------------------------------------------*/
#include "app/Logging/platform/include/log.h"
#include "srvlib/include/srvutil.h"

#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <syslog.h>

/*------------------------------------------------------------------------------*/
enum LogTo_t
{
    LOG_TO_NONE   = 0x00,
    LOG_TO_PRINTF = 0x01,
    LOG_TO_SYSLOG = 0x02
};

//static const LogTo_t logTo = (LogTo_t)(LOG_TO_PRINTF | LOG_TO_SYSLOG);
static LogTo_t logTo_ = LOG_TO_PRINTF;

/*------------------------------------------------------------------------------*/
void Log_setLogTo(unsigned int logTo)
{
    logTo_ = (LogTo_t)logTo;
}

/*------------------------------------------------------------------------------*/
static char* getLogTime(char* buffer, const int bufferSize)
{
    if (bufferSize > 0)
    {
        struct timeval tv;
        struct tm      tm;

        gettimeofday(&tv, NULL);

        const time_t t = tv.tv_sec;
        localtime_r(&t, &tm);

        Srv_Snprintf(buffer, bufferSize, "%02d:%02d:%02d.%03ld", tm.tm_hour, tm.tm_min, tm.tm_sec, tv.tv_usec / 1000L);
        buffer[bufferSize - 1] = '\0';
    }

    return buffer;
}

/*------------------------------------------------------------------------------*/
bool Log_Init()
{
    /* When running ELC/Unit/Component test:
     * Connect to syslog when application knows ELC is running. */
    return false;
}

int Log_Shutdown(void)
{
    return 0;
}

/*------------------------------------------------------------------------------*/
void Log_doLog(const Log::Level_t logLevel, int Facility, const char* identity, const char* buf)
{
    // This is passed to openlog() already, so we don't need to use it here.
    (void) identity;

    const int prio = logLevel > Log::Notice ? Log::Notice : logLevel;

    if (logTo_ & LOG_TO_SYSLOG)
    {
        syslog(Facility | prio, "%s", buf);
    }

    if (logTo_ & LOG_TO_PRINTF)
    {
        char time_buf[50];
        printf("[%s] %s\n", getLogTime(time_buf, sizeof(time_buf)), buf);
    }
}

/*------------------------------------------------------------------------------*/
