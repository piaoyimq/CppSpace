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

#ifndef APP_LOGGING_SRC_LOGGING_MEMORY_H_
#define APP_LOGGING_SRC_LOGGING_MEMORY_H_

#include "app/include/module_id.h"
#include "stdmacros.h"
#include <stdint.h>
#include <stdarg.h>

struct RingBuffer_t;

void Logging_memory_init(void);

struct RingBuffer_t* Logging_memory_create(const unsigned int size);
void Logging_memory_destroy(struct RingBuffer_t** buffer,
        const unsigned int size);

/** Logging_memory_add()
 *      TRACE or App_Log is used to store logs into internal memory. It is a
 *      way to store info in coredumps for later debug-purpose.
 * In:
 *      buffer          What internal memory buffer to store into
 *      moduleId        LIH_ID, PDP_ID, ...
 *      logMessage      Null-terminated string
 *      logMessageSize  Number of chars in logMessage + 1 for null-termination of string
 */
void Logging_memory_add(RingBuffer_t* const buffer,
        const App_ModuleId_t moduleId,
        const char* const logMessage,
        const uint16_t logMessageSize);

__attribute__ ((format(printf, 3, 0)))
void Logging_memory_doLog(RingBuffer_t* const buffer,
        const App_ModuleId_t moduleId,
        const char* format,
        va_list argList);

#endif /* APP_LOGGING_SRC_LOGGING_MEMORY_H_ */
