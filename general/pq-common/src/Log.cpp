/*
 * Log.cpp
 *
 *  Created on: Feb 6, 2018
 *      Author: piaoyimq
 */


#include "pq-common/include/Log.h"
#include <fstream>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/utility/record_ordering.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/filesystem.hpp>

namespace logging = boost::log;
namespace attrs = boost::log::attributes;
namespace sinks = boost::log::sinks;
namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;


Log* Log::_instance = nullptr;

Log::AutoRelease Log::release;


BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", Log::SeverityLevel)
BOOST_LOG_ATTRIBUTE_KEYWORD(channel, "Channel", std::string)
BOOST_LOG_ATTRIBUTE_KEYWORD(scope, "Scope", attrs::named_scope::value_type)


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

    logging::add_console_log
    (
        std::clog,
        keywords::filter = severity <= (consoleSeverity <= minSeverity ? consoleSeverity : minSeverity),  //this consoleSeverity must <= minSeverity, else use minSeverity
//        keywords::filter = min_severity || severity >= critical,
        keywords::format =
                expr::format("%1% %2% [%3%|%4%] %5% %6%")
                                      % expr::format_date_time< boost::posix_time::ptime >("time-stamp", "%H:%M:%S.%f")
                                      % severity
                                      % expr::attr< pid_t >("process-id")
                                      % expr::attr< pid_t >("thread-id")
                                      % expr::format_named_scope(scope, keywords::format = "%n/%f:%l")
                                      % expr::smessage
    );

    previousLog.emplace(std::make_pair(Notice, std::string("The log file is: ") + createLogFile()));

}


void Log::initSyncFileLog(const std::string& filename, LogType logType)
{
    const char* logFilename = (filename.size() == 0 ? defaultLogFilename.c_str() : filename.c_str());
    size_t rotationSize = 100 * 1024 * 1024;
    auto rotationTimePoint = sinks::file::rotation_at_time_point(0, 0, 0);

    if (SingleHost == logType)
    {
        logging::add_file_log
        (
            keywords::file_name = logFilename,                                        /*< file name pattern >*/
            keywords::rotation_size = rotationSize,                                   /*< rotate files every 10 MiB... >*/
            keywords::time_based_rotation = rotationTimePoint, /*< ...or at midnight >*/
            keywords::format = expr::format("%1% %2% %3%[%4%|%5%] %6% <%7%> %8%")
                % expr::format_date_time< boost::posix_time::ptime >("time-stamp", "%Y-%m-%d %H:%M:%S.%f")
                % severity
                % expr::attr< std::string >("process-name")
                % expr::attr< pid_t >("process-id")
                % expr::attr< pid_t >("thread-id")
                % expr::attr< uint32_t >("sequence-id")
                % channel
                % expr::smessage
         );
    }
    else
    {
        logging::add_file_log
        (
            keywords::file_name = logFilename,                                        /*< file name pattern >*/
            keywords::rotation_size = rotationSize,                                   /*< rotate files every 10 MiB... >*/
            keywords::time_based_rotation = rotationTimePoint, /*< ...or at midnight >*/
            keywords::format = expr::format("%1% %2% %3% %4%[%5%|%6%] %7% <%8%> %9%")
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
    }

    logging::core::get()->set_filter(severity <= minSeverity);

    logging::core::get()->add_global_attribute("time-stamp", attrs::local_clock());

    char hostname[32];

    if( gethostname(hostname,sizeof(hostname)) )
    {
        previousLog.emplace(std::make_pair(Warning, "gethostname calling failed."));
    }

    logging::add_common_attributes();

    logging::core::get()->add_global_attribute("hostname", attrs::constant<std::string>(hostname));
    logging::core::get()->add_global_attribute("process-name", attrs::current_process_name());
    logging::core::get()->add_global_attribute("process-id", attrs::constant<pid_t>(getpid()));
    logging::core::get()->add_global_attribute("thread-id", attrs::mutable_constant<pid_t>(gettid()));
    logging::core::get()->add_global_attribute("sequence-id", attrs::counter< unsigned int >());
    logging::core::get()->add_global_attribute("Scope", attrs::named_scope());

}


void Log::initAsyncFileLog() //TODO
{
#if 1
    // Open a rotating text file
     boost::shared_ptr< std::ostream > strm(new std::ofstream("test.log"));
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

     boost::shared_ptr< sink_t > sink(new sink_t(
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

     logging::core::get()->add_global_attribute("time-stamp", attrs::local_clock());

     char hostname[32];

     if( gethostname(hostname,sizeof(hostname)) )
     {
//      TRACE_WARNING("Log", "gethostname calling failed");
//         previousLog[Warning] = "gethostname calling failed.";
         previousLog.emplace(std::make_pair(Warning, "gethostname calling failed."));
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


Log::Log(LogType logType, bool enableConsoleLog, SeverityLevel fileSeverity, SeverityLevel consoleSeverity,
        const std::string& filename):
        consoleEnable(enableConsoleLog), minSeverity(fileSeverity)
{
    defaultLogFilename = createLogFile();

    if(enableConsoleLog)
    {
        initConsoleLog(consoleSeverity);
    }

#if 1
    initSyncFileLog(filename);
#else
    initAsncFileLog();
#endif
    printPreviousLog();
}


void Log::printPreviousLog()
{
    for(auto& log: previousLog)
    {

        LOG_TRACE_IMPL("Log", log.first, log.second);
    }
}


void Log::initInThread()
{
    logging::core::get()->add_thread_attribute("thread-id", attrs::mutable_constant<pid_t>(gettid())); // BOOST_LOG_SCOPED_THREAD_ATTR("thread-id", attrs::mutable_constant<pid_t>(gettid()));
    logging::core::get()->add_thread_attribute("sequence-id", attrs::counter< unsigned int >());
}


std::string Log::createLogFile()
{
    std::string directory;
    try
    {
        directory = std::string("/var/") + boost::log::aux::get_process_name() + std::string("/");
        boost::filesystem::create_directory(directory);//TODO: c++17 provide filesystem library
    }
    catch(std::exception& e)
    {
        directory = std::string("./") + boost::log::aux::get_process_name() + std::string("-log/");
        boost::filesystem::create_directory(directory);
    }

    return directory + boost::log::aux::get_process_name() + std::string("%N.log");
}
