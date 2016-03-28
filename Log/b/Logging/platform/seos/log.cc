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
* Short description: SEOS implementation
*
*
* -------------------------------------------------------------------------------------------------------------------*/
#include "app/Logging/platform/include/log.h"
#include "app/Logging/include/logging.h"
#include "srvlib/include/srvutil.h"

#ifdef __cplusplus
extern "C" {
#endif
#include <platform/oai_log.h>
#ifdef __cplusplus
}
#endif

#include <platform/events/event_asesdk.h>

#include <stdint.h>
#include <string.h>

/*------------------------------------------------------------------------------*/
#define SIZE_SYSLOG_STRING          (50)
#define MAX_OAI_LOGSTRING_LENGTH    (200)

/*------------------------------------------------------------------------------*/
bool Log_Init()
{
    /* Start the OAI log service */
    oai_log_init();

    /* SSR always connect to syslog  */
    // todo: do we need syslog on SSR? can we just skip it?
    return true;
}

/*------------------------------------------------------------------------------*/
int Log_Shutdown(void)
{
    /* closelog() will be done in desrtuctor in Logging */
    return 0;
}

/*------------------------------------------------------------------------------*/
static uint32_t log_EventFromLogLevel(const Log::Level_t logLevel)
{
    /* Mapping of log levels:

        JUNOS
       #define Log::Emergency   0   // system is unusable
       #define Log::Alert   1   // action must be taken immediately
       #define Log::Critical    2   // critical conditions
       #define Log::Error     3   // error conditions
       #define Log::Warning 4   // warning conditions
       #define Log::Notice  5   // normal but significant condition
       #define Log::Info    6   // informational
       #define Log::Debug   7   // debug-level messages

        SEOS
       #define EVNT_ASESDK_EMERG             0x06f00000
       #define EVNT_ASESDK_ALERT             0x26f00020
       #define EVNT_ASESDK_CRIT              0x46f00040
       #define EVNT_ASESDK_ERR               0x66f00060
       #define EVNT_ASESDK_WARNING           0x86f00080
       #define EVNT_ASESDK_NOTICE            0xa6f000a0
       #define EVNT_ASESDK_INFO              0xc6f000c0
       #define DBUG_ASESDK_GEN               0xe6f00000
     */

    uint32_t event;
    switch (logLevel)
    {
        case Log::Emergency:
            event = EVNT_ASESDK_EMERG;
            break;
        case Log::Alert:
            event = EVNT_ASESDK_ALERT;
            break;
        case Log::Critical:
            event = EVNT_ASESDK_CRIT;
            break;
        case Log::Error:
            event = EVNT_ASESDK_ERR;
            break;
        case Log::Warning:
            event = EVNT_ASESDK_WARNING;
            break;
        case Log::Notice:
        default: // INFO and below
            event = EVNT_ASESDK_NOTICE;
            break;
    }
    return event;
}

/*------------------------------------------------------------------------------*/
void Log_doLog(const Log::Level_t logLevel, int facility __attribute__ ((unused)), const char* identity, const char* logMessage)
{
    const Log::Level_t logLevelToUse = logLevel > Log::Notice ? Log::Notice : logLevel;

    const uint32_t     event         = log_EventFromLogLevel(logLevelToUse);

    /* GGSN00068550
     * The log append should probably be done in app.c (see APP_MAX_LOG_STR) where we keep the legacy log append for
     * juniper, but since this should only be done for SEOS, and I do not want to rewrite the legacy parts for now,
     * I made the changes here for now.. */
    const size_t logMessageLen = strlen(logMessage);
    if (logMessageLen > MAX_OAI_LOGSTRING_LENGTH)
    {
        const char*  logMessagePtr = logMessage;
        char         buffer[MAX_OAI_LOGSTRING_LENGTH + 1];
        unsigned int buffer_len    = sizeof(buffer) - 1;

        /* logMessage is always starting with a sequence number, we want to keep this for all "split" messages:
         * logMessage = "123 ....." */
        unsigned int seqNoLen = 0;

        while (logMessagePtr < (logMessage + logMessageLen))
        {
            /* Might be a bit inefficient to copy into new buffer instead of just using pointers in old buffer, but
             * these super long messages are rare, and the copy only occurs for those messages, so keep as is for now */
            const unsigned int remaingChars = logMessageLen - (logMessagePtr - logMessage);
            if (remaingChars > buffer_len)
            {
                const char appendString[] = { " ..." };

                /* maximum number of chars we can fit for this log-message */
                const unsigned int numberOfCharsToCopy = buffer_len - (sizeof(appendString) - 1);

                /* add log-message to buffer, the first time, the log-message already contain the sequence-number */
                Srv_Strncpy(&buffer[seqNoLen], logMessagePtr, numberOfCharsToCopy);

                /* now append " ..." at end */
                Srv_Strncat(&buffer[seqNoLen], appendString, buffer_len - strlen(&buffer[seqNoLen]));
                logMessagePtr += numberOfCharsToCopy - 1; //-1 is due to the appended null char in Src_Strncpy

                /* when we have logged the first row, keep the sequence number in buffer[] so that all split
                 * logs will get the same sequence number */
                if (seqNoLen == 0)
                {
                    /* this will make seqNoLen=4 with example string above */
                    while (isdigit(logMessage[seqNoLen++]))
                        ;    /* loop until we hit something other that a digit */

                    /* reduce number of digits remaining from &buffer[seqNoLen] to end of buffer */
                    buffer_len -= seqNoLen;
                }
            }
            else
            {
                Srv_Strncpy(&buffer[seqNoLen], logMessagePtr, buffer_len);
                logMessagePtr += buffer_len + 1;
            }
            oai_log(event, "%s %s", identity, buffer);
        }
    }
    else
    {
        oai_log(event, "%s %s", identity, logMessage);
    }
}

/*------------------------------------------------------------------------------*/
