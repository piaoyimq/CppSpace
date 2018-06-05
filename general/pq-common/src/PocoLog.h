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
#include <memory>
//#include "pq-common.h"
#include "Poco/Logger.h"
#include "Poco/Message.h"
#include "Poco/LogStream.h"
#include "Poco/AutoPtr.h"
#include "Poco/SplitterChannel.h"
#include "Poco/FileChannel.h"


class Log
{
public:
    enum LogType
    {
        SingleProcess,
        MutipleProcess
    };


    static Log *instance(const std::string& filename="", Poco::Message::Priority fileSeverity = Poco::Message::PRIO_INFORMATION,
            bool enableConsoleLog = false, Poco::Message::Priority consoleSeverity = Poco::Message::PRIO_WARNING, LogType logType=SingleProcess)
    {
        if (nullptr == _instance)
        {
            _instance = new Log(filename, fileSeverity, enableConsoleLog, consoleSeverity, logType);
        }

        return _instance;
    }

    std::unique_ptr<Poco::LogStream>& getFileStream()
    {
        return _fileStream;
    }

    std::unique_ptr<Poco::LogStream>& getConsoleStream()
    {
        return _consoleStream;
    }


private:
    void initConsoleLog(Poco::Message::Priority consoleSeverity, const std::string& filename);

    void initFileLog(const std::string& filename, LogType logType, Poco::Message::Priority fileSeverity);

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
//    Log(LogType logType, bool enableConsoleLog, Message::Priority fileSeverity, Message::Priority consoleSeverity, const std::string& filename);
    Log(const std::string& filename, Poco::Message::Priority fileSeverity, bool enableConsoleLog, Poco::Message::Priority consoleSeverity, LogType logType);

    //no assign
    Log(const Log& log){}

    Log& operator=(const Log& log){}


    ~Log()
    {
    }

    /****attribute****/

    std::multimap<Poco::Message::Priority, std::string> previousLog;

    static Log *_instance;

    std::unique_ptr<Poco::LogStream> _consoleStream;

    std::unique_ptr<Poco::LogStream> _fileStream;

    static AutoRelease release;
};



#define LOG_TRACE_IMPL(severity, msg) \
    if(nullptr != Log::instance()->getConsoleStream()) \
    { \
        Log::instance()->getConsoleStream()->severity() << msg << std::endl; \
    } \
    if(nullptr != Log::instance()->getFileStream()) \
    { \
        Log::instance()->getFileStream()->severity() << msg << std::endl; \
    }



#define TRACE_FATAL(msg)                    LOG_TRACE_IMPL(fatal, msg)

#define TRACE_CRITICAL(msg)                 LOG_TRACE_IMPL(critical, msg)

#define TRACE_ERROR(msg)                    LOG_TRACE_IMPL(error, msg)

#define TRACE_WARNING(msg)                  LOG_TRACE_IMPL(warning, msg)

#define TRACE_NOTICE(msg)                   LOG_TRACE_IMPL(notice, msg)

#define TRACE_INFO(msg)                     LOG_TRACE_IMPL(information, msg)

#define TRACE_DEBUG(msg)                    LOG_TRACE_IMPL(debug, msg)




#endif /* GENERAL_PQ_COMMON_INCLUDE_POCOLOG_H_ */
