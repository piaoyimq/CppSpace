/*
 * Log.h
 *
 *  Created on: Feb 6, 2018
 *      Author: piaoyimq
 */

#ifndef GENERAL_PQ_COMMON_INCLUDE_LOG_H_
#define GENERAL_PQ_COMMON_INCLUDE_LOG_H_

#include <map>
#include <utility>
#include <sys/syscall.h>
#include <boost/log/common.hpp>
#include "pq-common.h"

namespace src = boost::log::sources;



//        sink->stop();  //TODO
//        sink->flush();

class Log
{
public:

    enum LogType
    {
        SingleHost,
        MutipleHost
    };

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

    static Log *instance(LogType logType=SingleHost, bool enableConsoleLog = false, SeverityLevel fileSeverity = Notice,
            SeverityLevel consoleSeverity = Warning, const std::string& filename = createLogFile())
    {
        if (nullptr == _instance)
        {
            _instance = new Log(logType, enableConsoleLog, fileSeverity, consoleSeverity, filename);
        }
        return _instance;
    }

    static void initInThread();

    SeverityLevel getMinSeverity()
    {
        return minSeverity;
    }

    bool isConsoleEnable()
    {
        return consoleEnable;
    }

private:

    void initConsoleLog(SeverityLevel consoleSeverity = Warning);

    void initSyncFileLog(const std::string& filename, LogType logType = SingleHost);

    void initAsyncFileLog();

    static std::string createLogFile();

    void printPreviousLog();

    struct AutoRelease
    {
        ~AutoRelease()
        {
            delete _instance;
            _instance = nullptr;
        }
    };

    //no copy
    Log(LogType logType, bool enableConsoleLog, SeverityLevel fileSeverity, SeverityLevel consoleSeverity, const std::string& filename);

    //no assign
    Log(const Log& log){}

    Log& operator=(const Log& log){}


    ~Log(){}

    /****attribute****/

    SeverityLevel minSeverity;

    std::string defaultLogFilename;

    bool consoleEnable;

    std::multimap<SeverityLevel, std::string> previousLog;

    static Log *_instance;

    static AutoRelease release;
};


template< typename CharT, typename TraitsT >
inline std::basic_ostream< CharT, TraitsT >& operator<< (std::basic_ostream< CharT, TraitsT >& strm,
        Log::SeverityLevel lvl)
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

namespace
{
typedef src::severity_channel_logger< Log::SeverityLevel, std::string > logger_type;
logger_type slg;

#define LOG_TRACE_IMPL(module, severity, msg) \
    if(severity <= Log::instance()->getMinSeverity()) \
    { \
        if(Log::instance()->isConsoleEnable()) \
        { \
            BOOST_LOG_NAMED_SCOPE(__FUNCTION__);\
            BOOST_LOG_CHANNEL_SEV(slg, module, severity) << msg; \
        } \
        else \
        { \
            BOOST_LOG_CHANNEL_SEV(slg, module, severity) << msg; \
        } \
    } \


#define TRACE_EMERGENCY(module, msg)        LOG_TRACE_IMPL(module, Log::Emergency, msg)

#define TRACE_ALERT(module, msg)            LOG_TRACE_IMPL(module, Log::Alert, msg)

#define TRACE_CRITICAL(module, msg)         LOG_TRACE_IMPL(module, Log::Critical, msg)

#define TRACE_ERROR(module, msg)            LOG_TRACE_IMPL(module, Log::Error, msg)

#define TRACE_WARNING(module, msg)          LOG_TRACE_IMPL(module, Log::Warning, msg)

#define TRACE_NOTICE(module, msg)           LOG_TRACE_IMPL(module, Log::Notice, msg)

#define TRACE_INFO(module, msg)             LOG_TRACE_IMPL(module, Log::Info, msg)

#define TRACE_DEBUG(module, msg)            LOG_TRACE_IMPL(module, Log::Debug, msg)

}
#endif /* GENERAL_PQ_COMMON_INCLUDE_LOG_H_ */
