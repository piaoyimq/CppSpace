/*
 * Client.cpp
 *
 *  Created on: Aug 04, 2017
 *      Author: zhuweibo
 */


#include "Client.h"




Client::Client(int timeoutSec) :
        _timeoutSec(timeoutSec)
{
    _setLogger();
}


void Client::_setLogger()
{
    if (!_logger.get())
    {
        _logger.reset(new Poco::LogStream(pu::Application::instance().logger()));
        poco_check_ptr(_logger);
    }
}


bool Client::_send(const std::string& data)
{
    int pos = 0;
    int segmentSize = 1000;
    std::string::size_type dataSize = data.size();
    const std::string prefix("^MESSG_FALG1\n");
    const std::string suffix("MESSG_FALG1$\n"); // have to send end flag '\n', else QT socket could not readline

    int n = write(_sockfd, prefix.c_str(), prefix.size());

    if (prefix.size() != n)
    {
        _logger->error() << "Send to pop up window failed" << std::endl;
        return false;
    }

    _logger->notice() << "____segment= start falg" << std::endl;

    while (pos < dataSize)
    {
        std::string segment = data.substr(pos, segmentSize);
        pos += segmentSize;
        _logger->debug() << "____segment=" << segment << std::endl;
        n = write(_sockfd, segment.c_str(), segment.size());

        if (segment.size() != n)
        {
            _logger->error() << "Send to pop up window failed" << std::endl;
            return false;
        }
    }


    n = write(_sockfd, suffix.c_str(), suffix.size());

    if (suffix.size() != n)
    {
        _logger->error() << "Send to pop up window failed" << std::endl;
        return false;
    }
    _logger->debug() << "____segment= end falg" << std::endl;
    return true;
}


bool Client::send(const std::string& data)
{
    const std::string prefix("^MESSG_FALG1");
    const std::string suffix("MESSG_FALG1$\n"); // have to send end flag '\n', else QT socket could not readline

//    const std::string wrapperData(prefix + data + suffix);
    const std::string wrapperData(data);

    const char* serverIp = "127.0.0.1";
    const int port = 4242;


    struct sockaddr_in serverAddr;

    if ((_sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        _logger->error() << "Cretate socket failed: " << strerror(errno) << std::endl;
        return false;
    }

    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(serverIp);

    if (connect(_sockfd, (struct sockaddr *) (&serverAddr), sizeof(struct sockaddr)) == -1)
    {
        _logger->error() << "Connect popup window server failed: " << strerror(errno) << std::endl;
        return false;
    }

    bool ret = _send(wrapperData);

    close(_sockfd);

    return ret;
}
