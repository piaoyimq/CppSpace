/*
 *          Copyright Andrey Semashev 2007 - 2015.
 * Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *          http://www.boost.org/LICENSE_1_0.txt)
 */
/*!
 * \file   main.cpp
 * \author Andrey Semashev
 * \date   30.08.2009
 *
 * \brief  An example of asynchronous logging in multiple threads.
 */

// #define BOOST_LOG_DYN_LINK 1

#include <stdexcept>
#include <string>
#include <iostream>
#include <fstream>
#include <functional>
#include <boost/ref.hpp>
#include <boost/bind.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
//#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/barrier.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/common.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/utility/record_ordering.hpp>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <boost/log/sources/severity_channel_logger.hpp>


#include <boost/log/common.hpp>
#include <boost/log/expressions.hpp>

#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

#include <boost/log/attributes/timer.hpp>
#include <boost/log/attributes/named_scope.hpp>

#include <boost/log/sources/logger.hpp>

#include <boost/log/support/date_time.hpp>

namespace logging = boost::log;
namespace attrs = boost::log::attributes;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;

using boost::shared_ptr;

enum
{
    LOG_RECORDS_TO_WRITE = 10,
    THREAD_COUNT = 2
};

pid_t gettid()
{
  return static_cast<pid_t>(::syscall(SYS_gettid));
}

enum severity_level
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

template< typename CharT, typename TraitsT >
inline std::basic_ostream< CharT, TraitsT >& operator<< (std::basic_ostream< CharT, TraitsT >& strm, severity_level lvl)
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
//src::severity_logger<severity_level> slg(warning);//default is warning
typedef src::severity_channel_logger< severity_level, std::string > logger_type;
logger_type slg;
//]

//[ example_wide_char_logging_initialization
// Declare attribute keywords
BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", severity_level)  //make sure the is "Severity"
BOOST_LOG_ATTRIBUTE_KEYWORD(channel, "Channel", std::string)


//#define DEFAULT_MIN_LEVEL warning
severity_level minSeverityLevel = Critical;

#define LOG_TRACE_IMPL(module, severity, msg) \
    if(severity <= minSeverityLevel) \
    { \
        BOOST_LOG_CHANNEL_SEV(slg, module, severity) << msg; \
    } \


#define TRACE_EMERGENCY(module, msg)        LOG_TRACE_IMPL(module, Emergency, msg)

#define TRACE_ALERT(module, msg)            LOG_TRACE_IMPL(module, Alert, msg)

#define TRACE_CRITICAL(module, msg)         LOG_TRACE_IMPL(module, Critical, msg)

#define TRACE_ERROR(module, msg)            LOG_TRACE_IMPL(module, Error, msg)

#define TRACE_WARNING(module, msg)          LOG_TRACE_IMPL(module, Warning, msg)

#define TRACE_NOTICE(module, msg)           LOG_TRACE_IMPL(module, Notice, msg)

#define TRACE_INFO(module, msg)             LOG_TRACE_IMPL(module, Info, msg)

#define TRACE_DEBUG(module, msg)            LOG_TRACE_IMPL(module, Debug, msg)



void initConsoleLog()
{
#if 0
    // Create a minimal severity table filter
    typedef expr::channel_severity_filter_actor< std::string, severity_level > min_severity_filter;
    min_severity_filter min_severity = expr::channel_severity_filter(channel, severity);

    // Set up the minimum severity levels for different channels
    min_severity["general"] = Notice;
    min_severity["network"] = Warning;
    min_severity["gui"] = Error;
#endif
#if 1
    logging::add_console_log
    (
        std::clog,
        keywords::filter = severity <= Error,  //this severity must <= minSeverityLevel
//        keywords::filter = min_severity || severity >= critical,
        keywords::format =
                expr::format("%1% %2% %3% %4%[%5%|%6%] %7% <%8%> %9%")
                                      % expr::format_date_time< boost::posix_time::ptime >("time-stamp", "%Y-%m-%d %H:%M:%S.%f")
                                      % severity
                                      % expr::attr< pid_t >("process-id")
                                      % expr::attr< pid_t >("thread-id")
                                      % channel
                                      % expr::smessage
    );
#endif
}


void initSyncFile()
{
    logging::add_file_log
    (
        keywords::file_name = "sample_%N.log",                                        /*< file name pattern >*/
        keywords::rotation_size = 10 * 1024 * 1024,                                   /*< rotate files every 10 MiB... >*/
        keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0), /*< ...or at midnight >*/
        keywords::format =
                    expr::format("%1% %2% %3% %4%[%5%|%6%] %7% <%8%> %9%")
                        % expr::format_date_time< boost::posix_time::ptime >("time-stamp", "%Y-%m-%d %H:%M:%S.%f")
                        % severity
                        % expr::attr< std::string >("hostname")
                        % expr::attr< std::string >("process-name")
                        % expr::attr< pid_t >("process-id")
                        % expr::attr< pid_t >("thread-id")
                        % expr::attr< uint32_t >("sequence-id")
                        % channel
                        % expr::smessage
    );

    logging::core::get()->set_filter(severity <= minSeverityLevel);

//    logging::core::get()->add_sink(sink);

    logging::core::get()->add_global_attribute("time-stamp", attrs::local_clock());

    char hostname[32];

    if( gethostname(hostname,sizeof(hostname)) )
    {
     std::cout << "gethostname calling error" << std::endl;
     return;
    }

    logging::core::get()->add_global_attribute("hostname", attrs::constant<std::string>(hostname));
    logging::core::get()->add_global_attribute("process-name", attrs::current_process_name());
    logging::core::get()->add_global_attribute("process-id", attrs::constant<pid_t>(getpid()));
    logging::core::get()->add_global_attribute("thread-id", attrs::mutable_constant<pid_t>(gettid()));
    logging::core::get()->add_global_attribute("sequence-id", attrs::counter< unsigned int >());

    logging::add_common_attributes();

}


void initAsyncFileLog()
{
    // Open a rotating text file
     shared_ptr< std::ostream > strm(new std::ofstream("test.log"));
     if (!strm->good())
         throw std::runtime_error("Failed to open a text log file");

     // Create a text file sink
     typedef sinks::text_ostream_backend backend_t;
     typedef sinks::asynchronous_sink<
         backend_t,
         sinks::unbounded_ordering_queue<
             logging::attribute_value_ordering< unsigned int, std::greater< unsigned int > >
         >
     > sink_t;

//     boost::shared_ptr< sinks::synchronous_sink< sinks::text_file_backend > > sink = logging::add_file_log

     shared_ptr< sink_t > sink(new sink_t(
         boost::make_shared< backend_t >(),
         // We'll apply record ordering to ensure that records from different threads go sequentially in the file
         keywords::order = logging::make_attr_ordering("sequence-id", std::greater< unsigned int >())));

     sink->locked_backend()->add_stream(strm);

//        logging->add_global_attribute("process-id", attr::current_process_id());
//        keywords::filter = severity >= warning;
//        keywords::filter = min_severity || severity >= critical,
     sink->set_formatter
     (
         expr::format("%1% %2% %3% %4%[%5%|%6%] %7% <%8%> %9%")
             % expr::format_date_time< boost::posix_time::ptime >("time-stamp", "%Y-%m-%d %H:%M:%S.%f")
             % severity
             % expr::attr< std::string >("hostname")
             % expr::attr< std::string >("process-name")
             % expr::attr< pid_t >("process-id")
             % expr::attr< pid_t >("thread-id")
             % expr::attr< uint32_t >("sequence-id")
             % channel
             % expr::smessage
     );
//        sink->set_filter(expr::attr< severity_level >("severity") >= normal);
     sink->set_filter(severity <= minSeverityLevel);
//     sink->set_filter(expr::attr< std::string >("Channel") == "net");
     // Add it to the core

     logging::core::get()->add_sink(sink);

     // Add some attributes too
     //% expr::attr< boost::posix_time::ptime >("time-stamp")
 //% expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d, %H:%M:%S.%f")
//        % expr::attr< attrs::current_process_id::value_type >("process-id")
//        % expr::attr< boost::thread::id >("thread-id")
     logging::core::get()->add_global_attribute("time-stamp", attrs::local_clock());

     char hostname[32];

     if( gethostname(hostname,sizeof(hostname)) )
     {
      std::cout << "gethostname calling error" << std::endl;
      return;
     }

     logging::core::get()->add_global_attribute("hostname", attrs::constant<std::string>(hostname));
     logging::core::get()->add_global_attribute("process-name", attrs::current_process_name());
     logging::core::get()->add_global_attribute("process-id", attrs::constant<pid_t>(getpid()));
     logging::core::get()->add_global_attribute("thread-id", attrs::mutable_constant<pid_t>(gettid()));
//        BOOST_LOG_SCOPED_THREAD_TAG("thread-id", gettid());
     logging::core::get()->add_global_attribute("sequence-id", attrs::counter< unsigned int >());

     logging::add_common_attributes();

}


void init()
{
    initConsoleLog();
    initSyncFile();
//    initAsncFile();
}

//! This function is executed in multiple threads
void thread_fun(boost::barrier& bar)
{
    // Wait until all threads are created
    bar.wait();
    // Here we go. First, identify the thread.
//    BOOST_LOG_SCOPED_THREAD_TAG("thread-id", boost::this_thread::get_id());
//    logging::core::get()->add_global_attribute("thread-id", attrs::constant<pid_t>(gettid()));
//    logging::core::get()->add_global_attribute("thread-id", attrs::mutable_constant<pid_t>(gettid()));
//    BOOST_LOG_SCOPED_THREAD_TAG("thread-id", gettid());
    BOOST_LOG_SCOPED_THREAD_ATTR("thread-id", attrs::mutable_constant<pid_t>(gettid()));
    logging::core::get()->add_thread_attribute("sequence-id", attrs::counter< unsigned int >());
    // Now, do some logging
    for (unsigned int i = 0; i < LOG_RECORDS_TO_WRITE; ++i)
    {
        TRACE_ERROR("thread-module", "Log record " << i);
    }
    std::cout << "branch-LWP=" << gettid() << std::endl;
//    sleep(5);
}



int main(int argc, char* argv[])
{
    try
    {
        init();

        std::cout << "main-LWP=" << gettid() << std::endl;

        for (unsigned int i = 0; i < 2; ++i)
        {
            TRACE_ERROR("modul1", "Log record main" << i);
        }

        // Create logging threads
        boost::barrier bar(THREAD_COUNT);
        boost::thread_group threads;
        for (unsigned int i = 0; i < THREAD_COUNT; ++i)
            threads.create_thread(boost::bind(&thread_fun, boost::ref(bar)));
//        keywords::filter = expr::attr< severity_level >("Severity") >= warning;


        TRACE_CRITICAL("module2", "test critical");
        pid_t p = fork();

          if (p == 0)
          {
            std::cout << "child pid=" << getpid() << std::endl;
            TRACE_CRITICAL("module2", "test critical");
            TRACE_NOTICE("module2", "test NOTICE");
            TRACE_DEBUG("module2", "test DEBUG");
//            sleep(5);
          }
          else
          {
              std::cout << "parent pid=" << getpid() << std::endl;
              TRACE_CRITICAL("module2", "test critical");
              TRACE_NOTICE("module2", "test NOTICE");
              TRACE_DEBUG("module2", "test DEBUG");
              TRACE_NOTICE("module2", "test NOTICE");
              TRACE_DEBUG("module2", "test NOTICE");
              TRACE_CRITICAL("module2", "test critical");

//              sleep(5);
          }

        // Wait until all action ends
        threads.join_all();

        for (unsigned int i = 0; i < 3; ++i)
        {
            TRACE_DEBUG("module2", "test debug="<< i);
        }

        // Flush all buffered records
//        sink->stop();
//        sink->flush();

        return 0;
    }
    catch (std::exception& e)
    {
        std::cout << "FAILURE: " << e.what() << std::endl;
        return 1;
    }
}
