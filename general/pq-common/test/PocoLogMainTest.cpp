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
#include <gtest/gtest.h>

#define LOG_FILE_NAME       "test.log"

const std::string processName(get_process_name(getpid())+ ".log");


class PocoLogTestSuite2: public ::testing::Test
{
protected:
    virtual void SetUp()  //excute before every TEST_F
    {
        _removeLog();
        PocoLog::release();
    }

    virtual void TearDown()//excute after every TEST_F
    {
        _removeLog();
        PocoLog::release();
    }


private:
    void _removeLog()
    {
        Poco::File defaultLog(processName);
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




};
int _debugLine;
int _infoLine;
int _noticeLine;
int _warningLine;
int _errorLine;
int _criticalLine;
int _fatalLine;
void _printLog()
{
    TRACE_DEBUG(__FUNCTION__ << " " << __LINE__);    _debugLine    = __LINE__;
    TRACE_INFO(__FUNCTION__ << " " << __LINE__);     _infoLine     = __LINE__;
    TRACE_NOTICE(__FUNCTION__ << " " << __LINE__);   _noticeLine   = __LINE__;
    TRACE_WARNING(__FUNCTION__ << " " << __LINE__);  _warningLine  = __LINE__;
    TRACE_ERROR(__FUNCTION__ << " " << __LINE__);    _errorLine    = __LINE__;
    TRACE_CRITICAL(__FUNCTION__ << " " << __LINE__); _criticalLine = __LINE__;
    TRACE_FATAL(__FUNCTION__ << " " << __LINE__);    _fatalLine    = __LINE__;
}

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


TEST_F(PocoLogTestSuite2, defaultLogging)
{
    EXPECT_FALSE(Poco::File(processName).exists());

    TRACE_DEBUG("PocoLogTest.logging");
    TRACE_INFO("PocoLogTest.logging"); std::string infoLine = std::to_string(__LINE__);
    TRACE_NOTICE("PocoLogTest.logging"); std::string noticeLine = std::to_string(__LINE__);
    TRACE_WARNING("PocoLogTest.logging");

    //TODO: how to check console output?

    // Check log file if exist Information log
    EXPECT_TRUE(Poco::File(processName).exists());

    std::ifstream file(processName);
    std::ostringstream ss;
    ss << file.rdbuf();
    std::string expertString("Information " +  std::to_string(getpid()) + "|" + std::to_string(get_tid()) + " <PocoLogMainTest:" + infoLine + "> PocoLogTest.logging");
    EXPECT_NE(std::string::npos, ss.str().find(expertString));
}

TEST_F(PocoLogTestSuite2, allLogLevel)
{


    std::thread t(_printLog);
    t.join();
}
