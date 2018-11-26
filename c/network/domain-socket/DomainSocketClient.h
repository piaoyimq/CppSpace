
#ifndef TESTCELL_V93K_SMT7_SRC_EVENT_MONITOR_DAEMON_POPUPWINDOWCLIENT_H_
#define TESTCELL_V93K_SMT7_SRC_EVENT_MONITOR_DAEMON_POPUPWINDOWCLIENT_H_


//#include "Poco/Net/StreamSocket.h"
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "Poco/LogStream.h"
#include "Poco/Util/Application.h"
#include <boost/smart_ptr.hpp>




#define     DOMAIN_SOCKET_PATH      "/var/tmp/"

namespace pu = Poco::Util;


class DomainSocketClient
{
public:

    enum MsgType //Note: impact with msgTypeString
    {
        MSG_1 = 0,
        LAST
    };

    DomainSocketClient(const std::string& serverSocketFile, MsgType msgType, const std::string& clientSocketFile = generateClientSocketFile());

    virtual ~DomainSocketClient()
    {
        close(_sockFd);
        unlink(_clientSocketFile.c_str());
    }

    static pid_t getTid()
    {
      return static_cast<pid_t>(::syscall(SYS_gettid));
    }

    static std::string generateClientSocketFile()
    {
        return  DOMAIN_SOCKET_PATH + boost::lexical_cast<std::string>(getTid());
    }

    static const char* getMsgTypeString(MsgType msgType)
    {
        const char* const msgTypeString[] =
        {
                "MSG_1",
        };

        return (msgType < (MsgType) sizeof(msgTypeString)) ? msgTypeString[msgType] : "UNKOWN_MSG_TYPE";
    }

    bool send(const std::string& data);

private:

    void _setLogger();


    // attributes

    boost::scoped_ptr<Poco::LogStream> _logger;

    int _sockFd;

    std::string _serverSocketFile;

    MsgType _msgType;

    std::string _clientSocketFile;
};
#endif /* TESTCELL_V93K_SMT7_SRC_EVENT_MONITOR_DAEMON_POPUPWINDOWCLIENT_H_ */
