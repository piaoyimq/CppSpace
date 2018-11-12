/*
 * Client.h
 *
 *  Created on: Aug 14, 2018
 *      Author: zhuweibo
 */

#ifndef _MESSG_FALG1_DAEMON_Client_H_
#define MESSG_FALG1_DAEMON_Client_H_


#include "Poco/Net/StreamSocket.h"
#include "Poco/LogStream.h"
#include "Poco/Util/Application.h"
#include <boost/smart_ptr.hpp>






class Client
{
public:

    Client(int timeoutSec = 3);

    virtual ~Client()
    {
    }

    bool send(const std::string& data);

    bool _send(const std::string& data);


private:

    void _setLogger();

    // attributes

    boost::scoped_ptr<Poco::LogStream> _logger;

    int _timeoutSec;

    int _sockfd;
};
#endif /* MESSG_FALG1_DAEMON_Client_H_ */
