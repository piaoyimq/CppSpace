#ifndef GTPBASIC_INCLUDE_LOG_CFG_H_
#define GTPBASIC_INCLUDE_LOG_CFG_H_
/*---------------------------------------------------------------------------------------------------------------------
 * Template Id: 20041119
 *
 * Copyright (C) 2015 by Ericsson AB
 * S - 125 26  STOCKHOLM
 * SWEDEN, tel int +46 10 719 00 00
 *
 * The copyright to the computer program herein is the property of Ericsson AB. The program may be used and/or copied
 * only with the written permission from Ericsson AB, or in accordance with the terms and conditions stipulated in the
 * agreement/contract under which the program has been supplied.
 *
 * All rights reserved.
 */

#include "app/include/app.h"
#include <stdint.h>

/*-------------------------------------------------------------------------------------------------------------------*/

/* Keep track of configured log levels */
struct LogCfg_t
{
    Log::Level_t level;             /* Global level 0-31, Log::Level_t, is used for modules that are on in moduleOn[] */
    uint8_t      moduleOn[LAST_ID]; /* bool, if true  module  has loglevel ^ turned on*/
};

/*-------------------------------------------------------------------------------------------------------------------*/

#endif /* GTPBASIC_INCLUDE_LOG_CFG_H_ */
