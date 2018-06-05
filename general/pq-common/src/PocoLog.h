/*
 * PocoLog.h
 *
 *  Created on: Jun 1, 2018
 *      Author: azhweib
 */

#ifndef GENERAL_PQ_COMMON_INCLUDE_POCOLOG_H_
#define GENERAL_PQ_COMMON_INCLUDE_POCOLOG_H_

#include <map>
#include <iostream>
//#include "pq-common.h"
#include "Poco/Logger.h"
#include "Poco/Message.h"
#include "Poco/LogStream.h"
#include "Poco/AutoPtr.h"
#include "Poco/SplitterChannel.h"
#include "Poco/FileChannel.h"

using Poco::SplitterChannel;
using Poco::Channel;

using Poco::Message;
using Poco::Logger;
using Poco::AutoPtr;

Logger* logger;

class Log
{
public:
    enum LogType
    {
        SingleHost,
        MutipleHost
    };


//    std::cout << "____function=" << __FUNCTION__ << ", line=" << __LINE__ << std::endl;
    static Log *instance(LogType logType=SingleHost, bool enableConsoleLog = false, Message::Priority fileSeverity = Message::PRIO_NOTICE,
            Message::Priority consoleSeverity = Message::PRIO_WARNING, const std::string& filename = createLogFile())
    {
        if (nullptr == _instance)
        {
            _instance = new Log(logType, enableConsoleLog, fileSeverity, consoleSeverity, filename);
        }

        return _instance;
    }


//    Message::Priority getMinSeverity()
//    {
//        return minSeverity;
//    }

    bool isConsoleEnable()
    {
        return consoleEnable;
    }


    Poco::LogStream* getLoggerStream()
    {
        return _ls;
    }


private:
    void initConsoleLog(Message::Priority consoleSeverity, AutoPtr<SplitterChannel>& splitterChannel);

    void initSyncFileLog(const std::string& filename, LogType logType, AutoPtr<SplitterChannel>& splitterChannel);

    void initAsyncFileLog();

    static std::string createLogFile();

    void printPreviousLog();

    struct AutoRelease
    {
        ~AutoRelease()
        {
            delete _instance;
            _instance = nullptr;
        }
    };

    //no copy
    Log(LogType logType, bool enableConsoleLog, Message::Priority fileSeverity, Message::Priority consoleSeverity, const std::string& filename);

    //no assign
    Log(const Log& log){}

    Log& operator=(const Log& log){}


    ~Log(){}

    /****attribute****/

    Message::Priority minSeverity;

    std::string defaultLogFilename;

    bool consoleEnable;

    std::multimap<Message::Priority, std::string> previousLog;

    static Log *_instance;

    Poco::LogStream* _ls;

    static AutoRelease release;
};



#define LOG_TRACE_IMPL(severity, msg)       Log::instance()->getLoggerStream()->severity() << msg << std::endl;

#define TRACE_FATAL(msg)                    LOG_TRACE_IMPL(fatal, msg)

#define TRACE_CRITICAL(msg)                 LOG_TRACE_IMPL(critical, msg)

#define TRACE_ERROR(msg)                    LOG_TRACE_IMPL(error, msg)

#define TRACE_WARNING(msg)                  LOG_TRACE_IMPL(warning, msg)

#define TRACE_NOTICE(msg)                   LOG_TRACE_IMPL(notice, msg)

#define TRACE_INFO(msg)                     LOG_TRACE_IMPL(information, msg)

#define TRACE_DEBUG(msg)                    LOG_TRACE_IMPL(debug, msg)




#endif /* GENERAL_PQ_COMMON_INCLUDE_POCOLOG_H_ */
