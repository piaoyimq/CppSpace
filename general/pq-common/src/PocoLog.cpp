/*
 * Log.cpp
 *
 *  Created on: Jun 5, 2018
 *      Author: azhweib
 */

#include "PocoLog.h"
#include "Poco/ConsoleChannel.h"
#include "Poco/PatternFormatter.h"
#include "Poco/FormattingChannel.h"
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <fstream>




Log* Log::_instance = nullptr;

Log::AutoRelease Log::release;



void Log::initConsoleLog(Poco::Message::Priority consoleSeverity, const std::string& filename)
{
    Poco::FormattingChannel* pFCConsole = new Poco::FormattingChannel(new Poco::PatternFormatter("%H:%M:%S.%c  %p: %t"));//this pointer will be release in the wrapper class
    pFCConsole->setChannel(new Poco::ColorConsoleChannel);
    pFCConsole->open();
    Poco::Logger& consoleLogger = Poco::Logger::create("ConsoleLogger", pFCConsole, consoleSeverity);
    _consoleStream = std::unique_ptr<Poco::LogStream>(new Poco::LogStream(consoleLogger));

    if(filename != "")
    {
        _consoleStream->notice() << "Detail log reference \"" << filename << "\""<< std::endl;
    }
}


void Log::initFileLog(const std::string& filename, LogType logType, Poco::Message::Priority fileSeverity)
{
    Poco::FileChannel* fileChannel = new Poco::FileChannel(filename);

    fileChannel->setProperty("rotation", "100 M");
    fileChannel->setProperty("archive", "number");  //fileChannel->setProperty("archive", "timestamp");
    fileChannel->setProperty("purgeCount", "5");
    fileChannel->setProperty("compress", "true");

    //[%p %Y-%m-%d %H:%M:%S %i %P %I]\n%U:%u\n%t
    //%Y-%m-%d %H:%M:%S.%c %N[%P]:%s:%q:%t
    //%Y-%m-%d %H:%M:%S.%c %p %t
    Poco::FormattingChannel* pFCFile = new Poco::FormattingChannel(new Poco::PatternFormatter("%Y-%m-%d %H:%M:%S.%c %p %t"));

    pFCFile->setChannel(fileChannel);

    pFCFile->open();

    Poco::Logger& fileLogger = Poco::Logger::create("FileLogger", pFCFile, fileSeverity);

    _fileStream = std::unique_ptr<Poco::LogStream>(new Poco::LogStream(fileLogger));
}


Log::Log(const std::string& filename, Poco::Message::Priority fileSeverity, bool enableConsoleLog,
        Poco::Message::Priority consoleSeverity, LogType logType):
        _fileStream(nullptr), _consoleStream(nullptr)
{
    if(enableConsoleLog)
    {
        initConsoleLog(consoleSeverity, filename);
    }

    if(filename!="")
    {
        initFileLog(filename, logType, fileSeverity);
    }

//    printPreviousLog();
}


void Log::printPreviousLog()
{
//    for(auto& log: previousLog)
//    {
//
//        LOG_TRACE_IMPL("Log", log.first, log.second);
//    }
}


int main (int, char**)
{
//    Log::instance(); // log nothing
//    Log::instance("", Message::PRIO_TRACE, true, Message::PRIO_TRACE);// only print console log
    Log::instance("my-test.log", Poco::Message::PRIO_TRACE, true, Poco::Message::PRIO_TRACE);// print file log and console log

    for(int i=0 ; i < 10; i++)
    {
        TRACE_DEBUG("line=" << __LINE__);
        TRACE_INFO("line=" << __LINE__);
        TRACE_NOTICE("line=" << __LINE__);
        TRACE_WARNING("line=" << __LINE__);
        TRACE_ERROR("line=" << __LINE__);
        TRACE_CRITICAL("line=" << __LINE__);
        TRACE_FATAL("line=" << __LINE__);
    }

	return 0;
}

