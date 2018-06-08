/*
 * PocoLogTest.cpp
 *
 *  Created on: Jun 6, 2018
 *      Author: azhweib
 */



#include <fstream>
#include <sstream>
#include <thread>
#include <cassert>
#include "Poco/File.h"
#include "Poco/Process.h"
#include "Poco/Pipe.h"
#include "Poco/PipeStream.h"
#include "Poco/StreamCopier.h"
#include "pq-common/include/PocoLog.h"
#if 1
#include <gtest/gtest.h>

#define LOG_FILE_NAME       "test.log"

//const char* processName =
class PocoLogTestSuite2: public ::testing::Test
{
protected:
    void removeLog()
    {
        Poco::File defaultLog(get_process_name(getpid())+ ".log");
        if(defaultLog.exists())
        {
            defaultLog.remove();
        }

        Poco::File defineLog(LOG_FILE_NAME);
        if(defineLog.exists())
        {
            defineLog.remove();
        }
    }

    virtual void SetUp()  //excute before every TEST_F
    {
        removeLog();
    }

    virtual void TearDown()
    {
        removeLog();
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
TEST(PocoLogTestSuite1, nullPtr)
{
    EXPECT_EQ(PocoLog::instance(), PocoLog::instance());
    EXPECT_TRUE(PocoLog::instance()->getFileStream());
    EXPECT_TRUE(PocoLog::instance()->getConsoleStream());
    PocoLog::release();
}


TEST(PocoLogTestSuite1, onlyOutputConsoleLog)
{
    auto itsPocoLog = PocoLog::instance("", Poco::Message::PRIO_INFORMATION, true, Poco::Message::PRIO_NOTICE);
    EXPECT_FALSE(PocoLog::instance()->getFileStream());
    EXPECT_TRUE(PocoLog::instance()->getConsoleStream());
    PocoLog::release();
}


TEST(PocoLogTestSuite1, onlyOutputFileLog)
{
    auto itsPocoLog = PocoLog::instance(LOG_FILE_NAME, Poco::Message::PRIO_INFORMATION, false);
    EXPECT_TRUE(itsPocoLog);
    EXPECT_TRUE(PocoLog::instance()->getFileStream());
    EXPECT_FALSE(PocoLog::instance()->getConsoleStream());
    PocoLog::release();
}


TEST(PocoLogTestSuite1, outputFileLogAndConsoleLog)
{
    auto itsPocoLog = PocoLog::instance(LOG_FILE_NAME, Poco::Message::PRIO_INFORMATION, true);
    EXPECT_TRUE(PocoLog::instance()->getFileStream());
    EXPECT_TRUE(PocoLog::instance()->getConsoleStream());
    PocoLog::release();
}


TEST(PocoLogTestSuite1, instance)
{
    PocoLog* itsLog = PocoLog::instance(LOG_FILE_NAME, Poco::Message::PRIO_TRACE, true);

    EXPECT_TRUE(itsLog);

    EXPECT_EQ(itsLog, PocoLog::instance());

    EXPECT_TRUE(PocoLog::instance()->getFileStream());

    EXPECT_TRUE(PocoLog::instance()->getConsoleStream());
    PocoLog::release();
}


TEST_F(PocoLogTestSuite2, logging)
{
    EXPECT_FALSE(Poco::File(get_process_name(getpid())+ ".log").exists());

    TRACE_DEBUG("PocoLogTest.logging");
    TRACE_INFO("PocoLogTest.logging"); std::string infoLine = std::to_string(__LINE__);
    TRACE_NOTICE("PocoLogTest.logging"); std::string noticeLine = std::to_string(__LINE__);
    TRACE_WARNING("PocoLogTest.logging");

    //TODO: how to check console output?

    // Check log file if exist Information log
    EXPECT_TRUE(Poco::File(get_process_name(getpid())+ ".log").exists());

    std::ifstream file(get_process_name(getpid())+ ".log");
    std::ostringstream ss;
    ss << file.rdbuf();
    std::string expertString("Information " +  std::to_string(getpid()) + "|" + std::to_string(get_tid()) + " <PocoLogMainTest:" + infoLine + "> PocoLogTest.logging");
    EXPECT_NE(std::string::npos, ss.str().find(expertString));

}

//TEST_F(LotTest, SingleHostSingleProcess)
//{
//    for(int i=0 ; i < 100; i++)
//    {
//        TRACE_DEBUG("line=" << __LINE__);
//        TRACE_INFO("line=" << __LINE__);
//        TRACE_NOTICE("line=" << __LINE__);
//        TRACE_WARNING("line=" << __LINE__);
//        TRACE_ERROR("line=" << __LINE__);
//        TRACE_CRITICAL("line=" << __LINE__);
//        TRACE_FATAL("line=" << __LINE__);
//    }
//
////    std::thread t(threadFunction);
////    t.join();
//}
#else
int main()
{
    {
        PocoLog::instance()->getFileStream();
        PocoLog::instance()->getConsoleStream();
        PocoLog::release();
    }

    {
        auto itsPocoLog = PocoLog::instance("", Poco::Message::PRIO_INFORMATION, true, Poco::Message::PRIO_NOTICE);
        std::cout << "____function=" << __FUNCTION__ << ", line=" << __LINE__ << ", itsPocoLog=" << itsPocoLog << std::endl;
        PocoLog::instance()->getFileStream();
        PocoLog::instance()->getConsoleStream();
        PocoLog::release();
    }

    {
        auto itsPocoLog = PocoLog::instance("my-test.log", Poco::Message::PRIO_INFORMATION, false);
        std::cout << "____function=" << __FUNCTION__ << ", line=" << __LINE__ << ", itsPocoLog=" << itsPocoLog << std::endl;
        PocoLog::instance()->getFileStream();
        PocoLog::instance()->getConsoleStream();
        PocoLog::release();
    }

    {
//        PocoLog::instance()->getFileStream();
//        PocoLog::instance()->getConsoleStream();
        std::cout << "____function=" << __FUNCTION__ << ", line=" << __LINE__ << std::endl;
        auto itsPocoLog = PocoLog::instance("my-test2.log", Poco::Message::PRIO_INFORMATION, true);
        std::cout << "____function=" << __FUNCTION__ << ", line=" << __LINE__ << ", itsPocoLog=" << itsPocoLog << std::endl;
//        assert(PocoLog::instance()->getFileStream());
//        assert(PocoLog::instance()->getConsoleStream());
        PocoLog::release();
    }

    std::string s("good");
    if("good" != s)
    {
        std::cout <<"not eq" << std::endl;
    }
}
#endif
