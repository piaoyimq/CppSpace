/*
 * Server.cpp
 *
 *  Created on: Nov 9, 2017
 *      Author: zhuweibo
 */
//#include "qglobal.h"
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <errno.h>
#include <boost/algorithm/string.hpp>
#include "Server.h"


int Server::getConnectionFd()
{
    int fd = _connFd;
    _connFd = -1;
    return fd;
}


void Server::start(void*(*handleCallback)(void*))
{
    _listenFd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_sockaddr;
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons(_port);
    server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    std::cout << "____Server start " << std::endl;
    if (bind(_listenFd, (struct sockaddr *) &server_sockaddr, sizeof(server_sockaddr)) == -1)
    {
        perror("bind");
    }

    if (listen(_listenFd, 20) == -1)
    {
        perror("listen");
        std::cout << __FUNCTION__ << __LINE__ << std::endl;
    }

    while (1)
    {
        struct sockaddr_in client_addr;
        socklen_t length = sizeof(client_addr);
        _connFd = accept(_listenFd, (struct sockaddr*) &client_addr, &length);
        if (_connFd < 0)
        {
            perror("accept");
        }
        std::cout << "____connFd=" << _connFd << std::endl;

        pthread_t recvThreadId;
        int ret = pthread_create(&recvThreadId, NULL, handleCallback, static_cast<void*>(this));
        if (ret)
        {
            std::cout << "Cretate receive pthread failed: " << strerror(errno) << std::endl;
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

    int fd = static_cast<Server*>(ptr)->getConnectionFd();

    pthread_t threadId = pthread_self();
    std::string filename(std::to_string(threadId) + ".log");
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

    if(boost::starts_with(buffer, "^MESSG_FALG1"))
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
    Server server(4242);
    server.start(handleData);

    return 0;
}

