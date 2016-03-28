#ifndef __LOG_H__
#define __LOG_H__
/*---------------------------------------------------------------------------------------------------------------------
* Template Id: 20041119
*
* Copyright (C) 2008 by Ericsson AB
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
* Short description:
*
*
* -------------------------------------------------------------------------------------------------------------------*/

/* --- Includes ----- */
#include "app/Logging/include/logging.h"

/* --- Typedefs ----- */

/* ====================================================================================================================
** INTERFACE FUNCTIONS
** ==================================================================================================================*/

#ifdef PCLAB
void Log_setLogTo(unsigned int logTo);
#endif

bool Log_Init();
int Log_Shutdown(void);

void Log_doLog(const Log::Level_t logLevel, int Facility, const char* identity, const char* buf);

#endif
