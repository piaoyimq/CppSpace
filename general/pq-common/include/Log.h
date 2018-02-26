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
#define FILE_NAME  boost::log::aux::get_process_name() + std::string("%N.log")

class Log
{
public:

    enum LogType
    {
        SingleHostSingleProcess,
        SingleHostMutipleProcess,
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

    static Log *instance(LogType logType=SingleHostSingleProcess, bool enableConsoleLog = false, SeverityLevel fileSeverity = Notice,
            SeverityLevel consoleSeverity = Warning, const std::string& filename = createLogDirectory() + FILE_NAME)
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

    void initSyncFileLog(const std::string& filename);

    void initAsyncFileLog();

    static std::string createLogDirectory();

    void printPreviousLog();

    struct AutoRelease //资源回收机制
    {
        /*
         * 这种方式提供的处理方式显然要比Singleton1的Free(), 来的要便捷，因为它依靠内部提供的Garbo嵌套类来提供服务，
         * 当Singleton类生命周期结束时，Garbo的类对象garbo_也要销毁， 它将调用析构函数,
         * 而在析构函数中又自动地释放了Singleton单例类申请的一些资源，这种实现就比较智能化。不需要手动释放资源。这是它的优势。
         * */
        ~AutoRelease()
        {
            delete _instance;
            _instance = nullptr;
            std::cout <<__PRETTY_FUNCTION__ << std::endl;
        }
    };

    //no copy
    Log(LogType logType=SingleHostSingleProcess, bool enableConsoleLog = false, SeverityLevel fileSeverity = Notice,
            SeverityLevel consoleSeverity = Warning, const std::string& filename = createLogDirectory() + FILE_NAME);

    //no assign
    Log(const Log& log){}

    Log& operator=(const Log& log){}


    ~Log(){}

    /****attribute****/

    SeverityLevel minSeverity;

    std::string defaultLogFilename;

    bool consoleEnable;

    std::multimap<SeverityLevel, std::string> previousLog;

    static Log *_instance; //引用性声明

    static AutoRelease release;  //引用性声明
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
