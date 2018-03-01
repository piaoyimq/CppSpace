/*
 * log-main-test.cpp
 *
 *  Created on: 2018-03-01
 *      Author: piaoyimq
 */


#include "pq-common/include/Log.h"
#include <gtest/gtest.h>

class LotTest: public ::testing::Test
{
protected:
    static void SetUpTestCase()
    {
        Log::instance(Log::SingleHost, true, Log::Debug, Log::Notice);
    }

};

TEST_F(LotTest, SingleHost)
{
    TRACE_NOTICE("LotTest", "SingleHost");
}
#if 0
namespace attrs = boost::log::attributes;
namespace logging = boost::log;


enum
{
    LOG_RECORDS_TO_WRITE = 10,
    THREAD_COUNT = 3
};


void thread_fun(boost::barrier& bar)
{
    // Wait until all threads are created
    bar.wait();

    Log::initInThread();

    for (unsigned int i = 0; i < LOG_RECORDS_TO_WRITE; ++i)
    {
        TRACE_ERROR("thread-module", "Log record " << i);
    }
}


int main(int argc, char* argv[])
{
    try
    {
        Log::instance(Log::SingleHost, true, Log::Debug, Log::Notice);

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


        TRACE_CRITICAL("module2", "test critical");
        pid_t p = fork();

          if (p == 0)
          {
            TRACE_CRITICAL("module2", "test critical");
            TRACE_NOTICE("module2", "test NOTICE");
            TRACE_DEBUG("module2", "test DEBUG");
          }
          else
          {
              TRACE_CRITICAL("module2", "test critical");
              TRACE_NOTICE("module2", "test NOTICE");
              TRACE_DEBUG("module2", "test DEBUG");
              TRACE_NOTICE("module2", "test NOTICE");
              TRACE_DEBUG("module2", "test NOTICE");
              TRACE_CRITICAL("module2", "test critical");
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
#endif
