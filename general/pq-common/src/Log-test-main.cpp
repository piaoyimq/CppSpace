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

#include "../include/Log.h"
//#include "pq-common/include/Log.h"

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


enum
{
    LOG_RECORDS_TO_WRITE = 10,
    THREAD_COUNT = 2
};
Log lg;
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
    lg.initInThread();
    // Now, do some logging
    for (unsigned int i = 0; i < LOG_RECORDS_TO_WRITE; ++i)
    {
        TRACE_ERROR("thread-module", "Log record " << i);
    }
//    sleep(5);
}




int main(int argc, char* argv[])
{
    try
    {

        lg.initSingleProcessLog(true, Log::Debug, Log::Notice);

        TRACE_NOTICE("main", "main-LWP=" << gettid());

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
            TRACE_CRITICAL("module2", "test critical");
            TRACE_NOTICE("module2", "test NOTICE");
            TRACE_DEBUG("module2", "test DEBUG");
//            sleep(5);
          }
          else
          {
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
        TRACE_EMERGENCY("main", e.what());
        return 1;
    }
}
