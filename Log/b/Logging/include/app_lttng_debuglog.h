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
* -------------------------------------------------------------------------------------------------------------------*/

#include <lttng/tracepoint.h>

TRACEPOINT_EVENT(ericsson_epg_pgw_applog, debuglog,
        TP_ARGS(unsigned int, loglevel, const char*, module, const char*, message),
        TP_FIELDS(
                ctf_integer(unsigned int, loglevel, loglevel)
                ctf_string(module, module)
                ctf_string(message, message)
                )
        )
TRACEPOINT_LOGLEVEL(ericsson_epg_pgw_applog, debuglog, TRACE_DEBUG)

