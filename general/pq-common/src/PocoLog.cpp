//
// LogRotation.cpp
//
// This class demonstrates the Log rotation feature.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

#include "PocoLog.h"

#include "Poco/ConsoleChannel.h"


#include "Poco/PatternFormatter.h"
#include "Poco/FormattingChannel.h"
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <fstream>



using Poco::ConsoleChannel;
using Poco::FileChannel;
using Poco::FormattingChannel;
using Poco::Formatter;
using Poco::PatternFormatter;





Log* Log::_instance = nullptr;

Log::AutoRelease Log::release;



void Log::initConsoleLog(Message::Priority consoleSeverity, AutoPtr<SplitterChannel>& splitterChannel)
{
    AutoPtr<Channel> consoleChannel(new ConsoleChannel());
    splitterChannel->addChannel(consoleChannel);
}


void Log::initSyncFileLog(const std::string& filename, LogType logType, AutoPtr<SplitterChannel>& splitterChannel)
{
    AutoPtr<FileChannel> rotatedFileChannel(new FileChannel(filename));
    rotatedFileChannel->setProperty("rotation", "100");
    rotatedFileChannel->setProperty("archive", "number");
    rotatedFileChannel->setProperty("compress", "true");

    splitterChannel->addChannel(rotatedFileChannel);
}


Log::Log(LogType logType, bool enableConsoleLog, Message::Priority fileSeverity, Message::Priority consoleSeverity,
        const std::string& filename):
        consoleEnable(enableConsoleLog), minSeverity(fileSeverity)
{
//    defaultLogFilename = createLogFile();
    AutoPtr<SplitterChannel> splitterChannel=new SplitterChannel();
    if(enableConsoleLog)
    {
        initConsoleLog(consoleSeverity, splitterChannel);
    }

#if 1
    initSyncFileLog(filename, SingleHost, splitterChannel);
#else
    initAsncFileLog();
#endif

    AutoPtr<Formatter> formatter(new PatternFormatter("%Y-%m-%d %H:%M:%S.%c %N[%P]:%s:%q:%t"));
    AutoPtr<Channel> formattingChannel(new FormattingChannel(formatter, splitterChannel));

    Poco::Logger& logger = Logger::create("TestLog", formattingChannel, Message::PRIO_TRACE);


    _ls = new Poco::LogStream(Poco::Logger::get("TestLog"));

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



std::string Log::createLogFile()
{
    return "";
//    std::string directory;
//    try
//    {
//        directory = std::string("/var/") + boost::log::aux::get_process_name() + std::string("/");
//        boost::filesystem::create_directory(directory);//TODO: c++17 provide filesystem library
//    }
//    catch(std::exception& e)
//    {
//        directory = std::string("./") + boost::log::aux::get_process_name() + std::string("-log/");
//        boost::filesystem::create_directory(directory);
//    }
//
//    return directory + boost::log::aux::get_process_name() + std::string("%N.log");
}


int main (int, char**)
{
#if 1


    Log::instance(Log::SingleHost, true, Message::PRIO_TRACE, Message::PRIO_TRACE, "./my-test.log");

    TRACE_DEBUG("test debug" << __LINE__);
    TRACE_NOTICE("test notice" << __LINE__);
    TRACE_NOTICE(__LINE__);




#else
	AutoPtr<SplitterChannel> splitterChannel(new SplitterChannel());

	AutoPtr<Channel> consoleChannel(new ConsoleChannel());
	AutoPtr<Channel> fileChannel(new FileChannel("test.log"));
	AutoPtr<FileChannel> rotatedFileChannel(new FileChannel("rotated.log"));

	rotatedFileChannel->setProperty("rotation", "100");
	rotatedFileChannel->setProperty("archive", "number");
	rotatedFileChannel->setProperty("compress", "true");

	splitterChannel->addChannel(consoleChannel);
	splitterChannel->addChannel(fileChannel);
	splitterChannel->addChannel(rotatedFileChannel);


	AutoPtr<Formatter> formatter(new PatternFormatter("%h-%M-%S.%i: %t"));
	AutoPtr<Channel> formattingChannel(new FormattingChannel(formatter, splitterChannel));

	Logger& logger = Logger::create("TestLog", formattingChannel, Message::PRIO_TRACE);

	for (int i = 0; i < 100; ++i)
	{
		std::ostringstream oss;
        sleep(1);
		oss << "Value of i: " << i;
		logger.fatal(oss.str());
	}
#endif

	return 0;
}
