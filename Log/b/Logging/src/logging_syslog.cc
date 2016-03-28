#include "logging_syslog.h"
#include <syslog.h>

void Logging_Syslog_open(const char* identityString, const int facility)
{
    openlog(identityString, (LOG_NDELAY | LOG_PERROR), facility);
}

void Logging_Syslog_close(void)
{
    // Safe to call even if not opened.
    closelog();
}
