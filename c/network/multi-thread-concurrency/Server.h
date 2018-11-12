/*
 * Server.h
 *
 *  Created on: Nov 9, 2018
 *      Author: azhweib
 */

#ifndef TESTCELL_V93K_COMMON_Server_H_
#define TESTCELL_V93K_COMMON_Server_H_

//#include "Poco/LogStream.h"
//#include "Poco/Util/Application.h"
//#include <boost/smart_ptr.hpp>

class Server
{
public:
    Server(int port):_port(port)
    {
    }

    ~Server()
    {
        close(_listenFd);
    }

    void start(void*(*handleCallback)(void*));

    int getConnectionFd();

private:

    int _listenFd;

    int _connFd;

    int _port;
};

#endif /* TESTCELL_V93K_COMMON_Server_H_ */
