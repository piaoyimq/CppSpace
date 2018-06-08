/*
 * PocoLog.h
 *
 *  Created on: Jun 1, 2018
 *      Author: azhweib
 */

#ifndef GENERAL_PQ_COMMON_INCLUDE_POCOLOG_H_
#define GENERAL_PQ_COMMON_INCLUDE_POCOLOG_H_

#include <iostream>
#include <memory>
#include "pq-common/include/pq-common.h"
#include "Poco/Logger.h"
#include "Poco/Path.h"
#include "Poco/Message.h"
#include "Poco/LogStream.h"
#include "Poco/FileChannel.h"


class PocoLog
{
public:
    enum LogType
    {
        SingleHostSingleProcess,
        SingleHostMutipleProcess, //TODO
        MutilpleHostMutipleProcess //TODO
    };


    static PocoLog *instance(const std::string& filename="", Poco::Message::Priority fileSeverity = Poco::Message::PRIO_INFORMATION,
            bool enableConsoleLog = false, Poco::Message::Priority consoleSeverity = Poco::Message::PRIO_NOTICE, LogType logType=SingleHostSingleProcess)
    {
        if (nullptr == _instance)
        {
            if("" == filename && false == enableConsoleLog)
            {
                _instance = new PocoLog(get_process_name(getpid())+ ".log", fileSeverity, true, consoleSeverity, logType);
            }
            else
            {
                _instance = new PocoLog(filename, fileSeverity, enableConsoleLog, consoleSeverity, logType);
            }
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

    static void release()
    {
        Poco::Logger::shutdown();
        delete _instance;
        _instance = nullptr;
    }

private:
    void _initConsoleLog(Poco::Message::Priority consoleSeverity, const std::string& filename);

    void _initFileLog(const std::string& filename, LogType logType, Poco::Message::Priority fileSeverity);

    struct AutoRelease
    {
        ~AutoRelease()
        {
            release();
        }
    };

    PocoLog(const std::string& filename, Poco::Message::Priority fileSeverity, bool enableConsoleLog, Poco::Message::Priority consoleSeverity, LogType logType);

    PocoLog(const PocoLog& log){}

    PocoLog& operator=(const PocoLog& log){}


    ~PocoLog()
    {
    }

    /****attribute****/

    static PocoLog *_instance;

    std::unique_ptr<Poco::LogStream> _consoleStream;

    std::unique_ptr<Poco::LogStream> _fileStream;

    static AutoRelease _release;
};


#define CONSOLE_TRACE_IMPL(severity, msg) \
    if(nullptr != PocoLog::instance()->getConsoleStream()) \
    { \
        PocoLog::instance()->getConsoleStream()->severity() << get_tid() << " <" << Poco::Path(__FILE__).getBaseName() << ":" << __LINE__ << "> " <<msg << std::endl; \
    } \


#define FILE_TRACE_IMPL(severity, msg) \
    if(nullptr != PocoLog::instance()->getFileStream()) \
    { \
        PocoLog::instance()->getFileStream()->severity() << get_tid() << " <" << Poco::Path(__FILE__).getBaseName() << ":" << __LINE__ << "> " <<msg << std::endl; \
    }


#define LOG_TRACE_IMPL(severity, msg) \
        CONSOLE_TRACE_IMPL(severity, msg) \
        FILE_TRACE_IMPL(severity, msg)


#define TRACE_FATAL(msg)                    LOG_TRACE_IMPL(fatal, msg)

#define TRACE_CRITICAL(msg)                 LOG_TRACE_IMPL(critical, msg)

#define TRACE_ERROR(msg)                    LOG_TRACE_IMPL(error, msg)

#define TRACE_WARNING(msg)                  LOG_TRACE_IMPL(warning, msg)

#define TRACE_NOTICE(msg)                   LOG_TRACE_IMPL(notice, msg)

#define TRACE_INFO(msg)                     LOG_TRACE_IMPL(information, msg)

#define TRACE_DEBUG(msg)                    LOG_TRACE_IMPL(debug, msg)




#endif /* GENERAL_PQ_COMMON_INCLUDE_POCOLOG_H_ */
