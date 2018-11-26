#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#include <errno.h>
#include <boost/algorithm/string.hpp>
#include "DomainServer.h"
#include "DomainSocketClient.h"


int DomainServer::getConnectionFd()
{
    int fd = _connFd;
    _connFd = -1;
    return fd;
}


void DomainServer::start(void*(*handleCallback)(void*))
{
    struct sockaddr_un serverAddr, clientAddr;
    size_t len;

    if ((_listenFd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
    {
        perror("socket error");
        return;
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sun_family = AF_UNIX;
    strcpy(serverAddr.sun_path, _socketFile.c_str());
    len = offsetof(struct sockaddr_un, sun_path) + strlen(serverAddr.sun_path);

    unlink(_socketFile.c_str());

    if (bind(_listenFd, (struct sockaddr *) &serverAddr, len) < 0)
    {
        perror("bind error");
        return;
    }

    if (listen(_listenFd, 20) < 0)
    {
        perror("listen error");
        return;
    }

    while (1)
    {
        socklen_t len = sizeof(clientAddr);
        _connFd = accept(_listenFd, (struct sockaddr *) &clientAddr, &len);
        if(_connFd < 0)
        {
            perror("accept error");
            continue;
        }

        std::cout << "____connFd=" << _connFd << std::endl;

        pthread_t recvThreadId;
        int ret = pthread_create(&recvThreadId, NULL, handleCallback, static_cast<void*>(this));
        if (ret)
        {
            std::cout << "Cretate receive pthread failed: " << strerror(errno) << std::endl;
            continue;
        }

        std::cout << __FUNCTION__ << __LINE__ << std::endl;

        ret = pthread_detach(recvThreadId);
        if (ret)
        {
            std::cout << "Detach receive pthread failed: " << strerror(errno) << std::endl;
        }

        while (-1 != _connFd) // make sure thread has already started
        {
            std::cout << __FUNCTION__ << __LINE__ << ", usleep" << std::endl;
            usleep(200);
        }

        std::cout << __FUNCTION__ << __LINE__ << ", ____connFd=" << _connFd << std::endl;
    }
}

#include <fstream>
void *handleData(void* ptr)
{
    char buffer[1500];

    int fd = static_cast<DomainServer*>(ptr)->getConnectionFd();

    pthread_t threadId = pthread_self();
    std::string filename(std::to_string(fd) + ".log");
    std::ofstream of(filename.c_str());

    std::cout << "____Start handleData thread, connFd=" << fd<< ", log file is=" << filename << std::endl;

    memset(buffer, 0, sizeof(buffer));
    int len = recv(fd, buffer, sizeof(buffer), 0);
    if (len <= 0)
    {
        perror("recv:");
        close(fd);
        return NULL;
    }

    of << "____buffer=" << buffer << std::endl;

    if(boost::starts_with(buffer, DomainSocketClient::getMsgTypeString(DomainSocketClient::UI_REPORT_MONITOR)))
    {
        while (len > 0)
        {
            memset(buffer, 0, sizeof(buffer));
            len = recv(fd, buffer, sizeof(buffer), 0);

            of << "____buffer=" << buffer << std::endl;
        }
    }

    close(fd);

    return NULL;
}


int main()
{
    DomainServer server(std::string(MSG_1) + "test-server");
    server.start(handleData);

    return 0;
}

