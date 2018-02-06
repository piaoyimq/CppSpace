/*
 * Log.cpp
 *
 *  Created on: Feb 6, 2018
 *      Author: piaoyimq
 */



#include "../include/Log.h"
#include <fstream>
//#include <boost/smart_ptr/shared_ptr.hpp>
//#include <boost/log/support/date_time.hpp>
//#include <boost/log/utility/setup/common_attributes.hpp>
//#include <boost/log/expressions.hpp>
//#include <boost/log/attributes.hpp>
//#include <boost/log/sinks.hpp>
//#include <boost/log/utility/record_ordering.hpp>
//#include <boost/log/utility/setup/file.hpp>
//#include <boost/log/utility/setup/console.hpp>

namespace logging = boost::log;
namespace attrs = boost::log::attributes;
namespace sinks = boost::log::sinks;
namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;

using boost::shared_ptr;

Log::SeverityLevel Log::minSeverity = Notice;
const char* Log::defaultLogFilename = "sample_%N.log";
Log::logger_type Log::slg;







BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", Log::SeverityLevel)
BOOST_LOG_ATTRIBUTE_KEYWORD(channel, "Channel", std::string)



void Log::initConsoleLog(SeverityLevel consoleSeverity)
{
#if 0
    // Create a minimal severity table filter
    typedef expr::channel_severity_filter_actor< std::string, SeverityLevel > min_severity_filter;
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
        keywords::filter = severity <= (consoleSeverity <= minSeverity ? consoleSeverity : minSeverity),  //this consoleSeverity must <= minSeverity, else use minSeverity
//        keywords::filter = min_severity || severity >= critical,
        keywords::format =
                expr::format("%1% %2% [%3%|%4%] %5% %6%")
                                      % expr::format_date_time< boost::posix_time::ptime >("time-stamp", "%Y-%m-%d %H:%M:%S.%f")
                                      % severity
                                      % expr::attr< pid_t >("process-id")
                                      % expr::attr< pid_t >("thread-id")
                                      % channel
                                      % expr::smessage
    );
#endif
}


void Log::initSyncFileLog(const char* filename)
{
    logging::add_file_log
    (
        keywords::file_name = (filename == nullptr ? defaultLogFilename : filename),                                        /*< file name pattern >*/
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

    logging::core::get()->set_filter(severity <= minSeverity);

//    logging::core::get()->add_sink(sink);

    logging::core::get()->add_global_attribute("time-stamp", attrs::local_clock());

    char hostname[32];

    if( gethostname(hostname,sizeof(hostname)) )
    {
     std::cout << "gethostname calling error" << std::endl;
    }

    logging::core::get()->add_global_attribute("hostname", attrs::constant<std::string>(hostname));
    logging::core::get()->add_global_attribute("process-name", attrs::current_process_name());
    logging::core::get()->add_global_attribute("process-id", attrs::constant<pid_t>(getpid()));
    logging::core::get()->add_global_attribute("thread-id", attrs::mutable_constant<pid_t>(gettid()));
    logging::core::get()->add_global_attribute("sequence-id", attrs::counter< unsigned int >());

    logging::add_common_attributes();

}


void Log::initAsyncFileLog()
{
#if 1
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
//        sink->set_filter(expr::attr< Log::SeverityLevel >("severity") >= normal);
     sink->set_filter(severity <= minSeverity);
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

#endif
}


void Log::initSingleProcessLog(bool enableConsoleLog, SeverityLevel fileSeverity, SeverityLevel consoleSeverity,
        const char* filename)
{
    minSeverity = fileSeverity;

    if(enableConsoleLog)
    {
        initConsoleLog(consoleSeverity);
    }

#if 1
    initSyncFileLog(filename);
#else
    initAsncFileLog();
#endif
}


void Log::initInThread()
{
    BOOST_LOG_SCOPED_THREAD_ATTR("thread-id", attrs::mutable_constant<pid_t>(gettid()));
    logging::core::get()->add_thread_attribute("sequence-id", attrs::counter< unsigned int >());
}

