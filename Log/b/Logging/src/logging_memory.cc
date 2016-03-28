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

/* ------------------------------------------------------------------------- */
/* INCLUDES                                                                  */
/* ------------------------------------------------------------------------- */
#include "logging_memory.h"
#include "app/Logging/include/logging.h"
#include "srvlib/include/atomic.h"
#include "srvlib/include/srvutil.h"
#include "srvlib/include/srv_buffer.h"

#include <stdint.h>
#include <string.h>

/* ------------------------------------------------------------------------- */
/* INTERNAL STRUCTURES                                                       */
/* ------------------------------------------------------------------------- */
struct InternalLogItem_t
{
    /* use this to find the precious trace-log,
     * make sure to check we are still in buffer, or we wrapped */
    struct InternalLogItem_t* prevData;
    uint64_t                  timeStamp;
    App_ModuleId_t            moduleId;

    /* previous trace-log must be seqNo of current - 1, or we have wrapped */
    uint16_t sequenceNo;
    uint16_t totalSize;

    /* the logMessage itself, dynamic length
     * use logItemConstructor() to create this object */
    char logMessage[0];
};

/* pointers to the last internal memory logs */
static const uint32_t InternalMemoryLogVersion = 1;
static const uint32_t InternalMemoryLogMagic   = 0xDEADBEEF;
static struct
{
    uint32_t                  version;      /* for gdb script purposes */
    uint32_t                  magic;
    uint32_t                  index;
    uint32_t                  count;
    struct InternalLogItem_t* logItemList[1024];
} internalMemoryLog;

/* ------------------------------------------------------------------------- */
/* INTERNAL FUNCTIONS                                                        */
/* ------------------------------------------------------------------------- */
static uint64_t Logging_memory_getTimestamp(void)
{
    uint64_t result;

    /* previous format was [ms] (when we didn't have as much resolution
     * of the timer. Now when we have full resolution, we use [us].
     * This can be detected like this in GDB-scripts:
     *    Epoch time:
     *      1000000000 = 2001-09-09
     *      1455693253 = 2016-02-17 07:14:12 (GMT)
     *      1500000000 = 2017-07-14 04:40:00 (GMT)
     *      4500000000 = 2112-08-07 10:00:00 (GMT)
     *    Previous format [ms]:
     *      1455693253123 = 2016-02-17 07:14:12.123 (GMT)
     *      4500000000123 = 2112-08-07 10:00:00.123 (GMT)
     *    New format [us]:
     *      1455693253123456 = 2016-02-17 07:14:12.123456 (GMT)
     *      4500000000123456 = 2112-08-07 10:00:00.123456 (GMT)
     *
     * So for the two formats, the following intervals are in use:
     *    [ms] : 1000000000123..4500000000123
     *    [us] : 1000000000123456..4500000000123456
     */
    const struct timeval tv = Logging_getTimeofdayFn();
    result  = tv.tv_sec;
    result *= 1000000L;
    result += tv.tv_usec;

    return result;
}

/** logItemConstructor()
 *      Create a log-item. A log-item is stored in memory and contains some
 *      extra data (time, module, ...) about the log message itself.
 */
static __inline__ InternalLogItem_t* logItemConstructor(
        const App_ModuleId_t moduleId,
        const uint16_t sequenceNo,
        const char* const logMessage,
        const uint16_t logMessageSize)
{
    const uint16_t           totalSize = sizeof(InternalLogItem_t) + logMessageSize;
    InternalLogItem_t* const logItem   = (InternalLogItem_t*)new char[totalSize];

    logItem->prevData   = NULL;
    logItem->timeStamp  = Logging_memory_getTimestamp();
    logItem->moduleId   = moduleId;
    logItem->sequenceNo = sequenceNo;
    logItem->totalSize  = totalSize;
    memcpy(logItem->logMessage, logMessage, logMessageSize);

    return(logItem);
}

/* ------------------------------------------------------------------------- */
/* INTERFACE FUNCTIONS                                                       */
/* ------------------------------------------------------------------------- */
void Logging_memory_add(RingBuffer_t* const buffer,
        const App_ModuleId_t moduleId,
        const char* const logMessage,
        const uint16_t logMessageSize)
{
    /* buffer is initialized before main() is called, should this
     * be NULL here then this is because some module is logging in
     * static initialization (this is wrong, however, prevent crashing). */
    if (buffer)
    {
        static uint16_t    sequenceNo = 0;

        InternalLogItem_t* logItem    = (InternalLogItem_t*)logItemConstructor(
                moduleId, sequenceNo++, logMessage, logMessageSize);

        const char*              prevLogItem = NULL;
        InternalLogItem_t* const newLogItem  = (InternalLogItem_t*)Srv_BufferMemCpy(
                buffer, logItem, logItem->totalSize, &prevLogItem);
        if (newLogItem)
        {
            newLogItem->prevData = (struct InternalLogItem_t*)prevLogItem;

            // store logItem to loglist
            srvlib::atomic::FetchAdd(&internalMemoryLog.count, 1U);
            const uint32_t index        = srvlib::atomic::FetchAdd(&internalMemoryLog.index, 1U);
            const uint32_t logItemIndex = index % Array_Size(internalMemoryLog.logItemList);
            internalMemoryLog.logItemList[logItemIndex] = newLogItem;
        }

        delete[] (char*)logItem;
    }
}

__attribute__ ((format(printf, 3, 0)))
void Logging_memory_doLog(RingBuffer_t* const buffer,
        const App_ModuleId_t moduleId,
        const char* format,
        va_list argList)
{
    const size_t logMessageSize = LOG_MESSAGE_MAX_LENGTH;
    char         logMessage[logMessageSize];
    memset(logMessage, 0, sizeof(logMessageSize));

    /* format log-message */
    const size_t len = Srv_Vsnprintf(logMessage, logMessageSize, format, argList);

    /* now store this log in internal memory */
    Logging_memory_add(buffer, moduleId, logMessage, len + 1);
}

void Logging_memory_init(void)
{
    memset(&internalMemoryLog, 0, sizeof(internalMemoryLog));
    internalMemoryLog.version = InternalMemoryLogVersion;
    internalMemoryLog.magic   = InternalMemoryLogMagic;
}

struct RingBuffer_t* Logging_memory_create(const unsigned int size)
{
    return Srv_BufferCreate(size);
}

void Logging_memory_destroy(struct RingBuffer_t** buffer,
        const unsigned int size)
{
    Srv_BufferDestroy(buffer, size);
}
