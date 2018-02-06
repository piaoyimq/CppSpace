/*
 * Log.h
 *
 *  Created on: Feb 6, 2018
 *      Author: piaoyimq
 */

#ifndef GENERAL_PQ_COMMON_INCLUDE_LOG_H_
#define GENERAL_PQ_COMMON_INCLUDE_LOG_H_

#include <sys/syscall.h>
#include <boost/log/common.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/utility/record_ordering.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>

namespace src = boost::log::sources;

inline pid_t gettid()
{
  return static_cast<pid_t>(::syscall(SYS_gettid));
}


class Log
{
public:

    enum SeverityLevel
    {
        Emergency,       //   0       Emergency: system is unusable
        Alert,           //   1       Alert: action must be taken immediately
        Critical,        //   2       Critical: critical conditions
        Error,           //   3       Error: error conditions
        Warning,         //   4       Warning: warning conditions
        Notice,          //   5       Notice: normal but significant condition
        Info,            //   6       Informational: informational messages
        Debug            //   7       Debug: debug-level messages
    };

    typedef src::severity_channel_logger< SeverityLevel, std::string > logger_type;

    static void initSingleProcessLog(bool enableConsoleLog = false, SeverityLevel fileSeverity = Notice,
            SeverityLevel consoleSeverity = Warning, const char* filename = defaultLogFilename);

    static void initInThread();

//private:

    static void initConsoleLog(SeverityLevel consoleSeverity = Warning);

    static void initSyncFileLog(const char* filename);

    static void initAsyncFileLog();

    static SeverityLevel minSeverity;

    static const char* defaultLogFilename;

    static logger_type slg;
};


template< typename CharT, typename TraitsT >
inline std::basic_ostream< CharT, TraitsT >& operator<< (std::basic_ostream< CharT, TraitsT >& strm, Log::SeverityLevel lvl)
{
 static const char* const str[] =
 {
      "Emergency",
      "Alert",
      "Critical",
      "Error",
      "Warning",
      "Notice",
      "Info",
      "Debug"
 };

 if (static_cast< std::size_t >(lvl) < (sizeof(str) / sizeof(*str)))
 {
     strm << str[lvl];
 }
 else
 {
     strm << static_cast< int >(lvl);
 }

 return strm;
}


#define LOG_TRACE_IMPL(module, severity, msg) \
    if(severity <= Log::minSeverity) \
    { \
        BOOST_LOG_CHANNEL_SEV(Log::slg, module, severity) << msg; \
    } \


#define TRACE_EMERGENCY(module, msg)        LOG_TRACE_IMPL(module, Log::Emergency, msg)

#define TRACE_ALERT(module, msg)            LOG_TRACE_IMPL(module, Log::Alert, msg)

#define TRACE_CRITICAL(module, msg)         LOG_TRACE_IMPL(module, Log::Critical, msg)

#define TRACE_ERROR(module, msg)            LOG_TRACE_IMPL(module, Log::Error, msg)

#define TRACE_WARNING(module, msg)          LOG_TRACE_IMPL(module, Log::Warning, msg)

#define TRACE_NOTICE(module, msg)           LOG_TRACE_IMPL(module, Log::Notice, msg)

#define TRACE_INFO(module, msg)             LOG_TRACE_IMPL(module, Log::Info, msg)

#define TRACE_DEBUG(module, msg)            LOG_TRACE_IMPL(module, Log::Debug, msg)


#endif /* GENERAL_PQ_COMMON_INCLUDE_LOG_H_ */
