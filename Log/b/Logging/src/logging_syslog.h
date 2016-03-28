/*
 * logging_syslog.h
 *
 *  Created on: Jan 21, 2016
 *      Author: qpetkan
 */

#ifndef _LOGGING_SYSLOG_H_
#define _LOGGING_SYSLOG_H_

void Logging_Syslog_open(const char* identityString, const int facility);
void Logging_Syslog_close(void);

#endif /* LOGGING_SYSLOG_H_ */
