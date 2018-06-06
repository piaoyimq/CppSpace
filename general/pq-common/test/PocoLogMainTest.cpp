/*
 * PocoLogTest.cpp
 *
 *  Created on: Jun 6, 2018
 *      Author: azhweib
 */





#include <thread>
#include "pq-common/include/PocoLog.h"
#include <gtest/gtest.h>

class LotTest: public ::testing::Test
{
protected:
    static void SetUpTestCase()
    {
//        PocoLog::instance(); // log nothing
//        PocoLog::instance("", Message::PRIO_TRACE, true, Message::PRIO_TRACE);// only print console log
//        PocoLog::instance("my-test.log", Poco::Message::PRIO_TRACE);// print file log and console log
        PocoLog::instance("my-test.log", Poco::Message::PRIO_TRACE, true);// print file log and console log
    }


};

//void threadFunction()
//{
//    for(int i=0 ; i < 1; i++)
//    {
//        TRACE_DEBUG("line=" << __LINE__);
//        TRACE_INFO("line=" << __LINE__);
//        TRACE_NOTICE("line=" << __LINE__);
//        TRACE_WARNING("line=" << __LINE__);
//        TRACE_ERROR("line=" << __LINE__);
//        TRACE_CRITICAL("line=" << __LINE__);
//        TRACE_FATAL("line=" << __LINE__);
//    }
//}


TEST_F(LotTest, SingleHostSingleProcess)
{
    for(int i=0 ; i < 1; i++)
    {
        TRACE_DEBUG("line=" << __LINE__);
        TRACE_INFO("line=" << __LINE__);
        TRACE_NOTICE("line=" << __LINE__);
        TRACE_WARNING("line=" << __LINE__);
        TRACE_ERROR("line=" << __LINE__);
        TRACE_CRITICAL("line=" << __LINE__);
        TRACE_FATAL("line=" << __LINE__);
    }

//    std::thread t(threadFunction);
//    t.join();
}
