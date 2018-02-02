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

//BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(test_lg, src::logger_mt)
//[ example_wide_char_severity_level_definition
enum severity_level
{
    normal,
    notification,
    warning,
    error,
    critical
};

template< typename CharT, typename TraitsT >
inline std::basic_ostream< CharT, TraitsT >& operator<< (
    std::basic_ostream< CharT, TraitsT >& strm, severity_level lvl)
{
    static const char* const str[] =
    {
        "normal",
        "notification",
        "warning",
        "error",
        "critical"
    };
    if (static_cast< std::size_t >(lvl) < (sizeof(str) / sizeof(*str)))
        strm << str[lvl];
    else
        strm << static_cast< int >(lvl);
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
        BOOST_LOG_CHANNEL_SEV(slg, "thread-module", error) << "Log record " << i;
    }
    std::cout << "branch-LWP=" << gettid() << std::endl;
//    sleep(5);
}



int main(int argc, char* argv[])
{
    try
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
                logging::attribute_value_ordering< unsigned int, std::less< unsigned int > >
            >
        > sink_t;
        shared_ptr< sink_t > sink(new sink_t(
            boost::make_shared< backend_t >(),
            // We'll apply record ordering to ensure that records from different threads go sequentially in the file
            keywords::order = logging::make_attr_ordering("record-id", std::less< unsigned int >())));

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
        sink->set_filter(severity >= warning);
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
         return 1;
        }

        logging::core::get()->add_global_attribute("hostname", attrs::constant<std::string>(hostname));
        logging::core::get()->add_global_attribute("process-name", attrs::current_process_name());
        logging::core::get()->add_global_attribute("process-id", attrs::constant<pid_t>(getpid()));
        logging::core::get()->add_global_attribute("thread-id", attrs::mutable_constant<pid_t>(gettid()));
//        BOOST_LOG_SCOPED_THREAD_TAG("thread-id", gettid());
        logging::core::get()->add_global_attribute("sequence-id", attrs::counter< unsigned int >());

        logging::add_common_attributes();


        std::cout << "main-LWP=" << gettid() << std::endl;

        for (unsigned int i = 0; i < 2; ++i)
        {
            BOOST_LOG_CHANNEL_SEV(slg,"module1", error) << "Log record main" << i;
        }

        // Create logging threads
        boost::barrier bar(THREAD_COUNT);
        boost::thread_group threads;
        for (unsigned int i = 0; i < THREAD_COUNT; ++i)
            threads.create_thread(boost::bind(&thread_fun, boost::ref(bar)));
//        keywords::filter = expr::attr< severity_level >("Severity") >= warning;


        BOOST_LOG_CHANNEL_SEV(slg, "module2", critical) << "A fasdf A normal severity message, will not pass to the file";
        pid_t p = fork();

          if (p == 0)
          {
            std::cout << "child pid=" << getpid() << std::endl;
            BOOST_LOG_CHANNEL_SEV(slg, "child", notification) << "Bchild pid= " << getpid();
            BOOST_LOG_CHANNEL_SEV(slg, "child", critical) << "C A normal severity message, will not pass to the file";
            BOOST_LOG_CHANNEL_SEV(slg, "child", normal) << "D A normal severity message, will not pass to the file";
//            sleep(5);
          }
          else
          {
              std::cout << "parent pid=" << getpid() << std::endl;
              BOOST_LOG_CHANNEL_SEV(slg, "module3", warning) << "P1 parent pid= " << getpid();
              BOOST_LOG_CHANNEL_SEV(slg, "module3", critical) << "P2 A normal severity message, will not pass to the file";
              BOOST_LOG_CHANNEL_SEV(slg, "module3", normal) << "P3 A normal severity message, will not pass to the file";
              BOOST_LOG(slg) << "P4 A normal severity message, will not pass to the file";
              BOOST_LOG(slg) << "P5 A normal severity message, will not pass to the file";
//              sleep(5);
          }

        // Wait until all action ends
        threads.join_all();

        for (unsigned int i = 0; i < 3; ++i)
        {
            BOOST_LOG_CHANNEL_SEV(slg, "module3", warning) << "F Log record main-2 " << i;
        }

        // Flush all buffered records
        sink->stop();
        sink->flush();

        return 0;
    }
    catch (std::exception& e)
    {
        std::cout << "FAILURE: " << e.what() << std::endl;
        return 1;
    }
}
