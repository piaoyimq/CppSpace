/*
 * PocoLog.cpp
 *
 *  Created on: Jun 5, 2018
 *      Author: azhweib
 */


#include "pq-common/include/PocoLog.h"
#include "Poco/AutoPtr.h"
#include "Poco/ConsoleChannel.h"
#include "Poco/PatternFormatter.h"
#include "Poco/FormattingChannel.h"
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <fstream>




PocoLog* PocoLog::_instance = nullptr;

PocoLog::AutoRelease PocoLog::_release;



void PocoLog::_initConsoleLog(Poco::Message::Priority consoleSeverity, const std::string& filename)
{

    //these pointer will be release in their wrapper class, so won't lead to memory leak.
    Poco::AutoPtr<Poco::Formatter> patternFormatter(new Poco::PatternFormatter("%H:%M:%S.%i %p %P|%t"));
    Poco::AutoPtr<Poco::FormattingChannel> pFCConsole(new Poco::FormattingChannel(patternFormatter));
    Poco::AutoPtr<Poco::Channel> colorConsoleChannel(new Poco::ColorConsoleChannel);
    pFCConsole->setChannel(colorConsoleChannel);
    pFCConsole->open();

    Poco::Logger& consoleLogger = Poco::Logger::create("ConsoleLogger", pFCConsole, consoleSeverity);
    _consoleStream = std::unique_ptr < Poco::LogStream > (new Poco::LogStream(consoleLogger));

    if (filename != "")
    {
        _consoleStream->notice() << get_tid() << " <" << Poco::Path(__FILE__).getBaseName() << ":" << __LINE__ << "> " << "Detail log reference \""
                << filename << "\"" << std::endl;
    }
}


void PocoLog::_initFileLog(const std::string& filename, LogType logType, Poco::Message::Priority fileSeverity)
{
    Poco::AutoPtr<Poco::FileChannel> fileChannel(new Poco::FileChannel(filename));

    fileChannel->setProperty("rotation", "300 M");
    fileChannel->setProperty("archive", "number");  //fileChannel->setProperty("archive", "timestamp");
    fileChannel->setProperty("purgeCount", "3");
    fileChannel->setProperty("compress", "true");

    Poco::AutoPtr<Poco::Formatter> patternFormatter(new Poco::PatternFormatter("%L%Y-%m-%d %H:%M:%S.%i%z %p %P|%t"));
    Poco::AutoPtr<Poco::FormattingChannel> pFCFile(new Poco::FormattingChannel(patternFormatter));

    pFCFile->setChannel(fileChannel);

    pFCFile->open();

    Poco::Logger& fileLogger = Poco::Logger::create("FileLogger", pFCFile, fileSeverity);

    _fileStream = std::unique_ptr<Poco::LogStream>(new Poco::LogStream(fileLogger));
}


PocoLog::PocoLog(const std::string& filename, Poco::Message::Priority fileSeverity, bool enableConsoleLog,
        Poco::Message::Priority consoleSeverity, LogType logType):
        _fileStream(nullptr), _consoleStream(nullptr)
{
    if(enableConsoleLog)
    {
        _initConsoleLog(consoleSeverity, filename);
    }

    if(filename!="" && SingleHostSingleProcess == logType)
    {
        _initFileLog(filename, logType, fileSeverity);
    }
}

