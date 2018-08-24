/*
 * server2.cpp
 *
 *  Created on: Aug 20, 2018
 *      Author: azhweib
 */




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

#define MYPORT  8887
#define QUEUE   20
#define BUFFER_SIZE 1024

int main()
{
    ///??sockfd
    int server_sockfd = socket(AF_INET,SOCK_STREAM, 0);

    ///??sockaddr_in
    struct sockaddr_in server_sockaddr;
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons(MYPORT);
    server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    ///bind,????0,????-1
    if(bind(server_sockfd,(struct sockaddr *)&server_sockaddr,sizeof(server_sockaddr))==-1)
    {
        perror("bind");
        exit(1);
    }

    printf("??%d??\n",MYPORT);
    ///listen,????0,????-1
    if(listen(server_sockfd,QUEUE) == -1)
    {
        perror("listen");
        exit(1);
    }

    ///??????
    char buffer[BUFFER_SIZE];
    struct sockaddr_in client_addr;
    socklen_t length = sizeof(client_addr);

    printf("???????\n");
    ///?????????,????-1
    int conn = accept(server_sockfd, (struct sockaddr*)&client_addr, &length);
    if(conn<0)
    {
        perror("connect");
        exit(1);
    }
    printf("???????\n");

    while(1)
    {
        memset(buffer,0,sizeof(buffer));
        int len = recv(conn, buffer, sizeof(buffer),0);
        //?????exit???????,??
        if(strcmp(buffer,"exit\n")==0 || len<=0)
            break;
        printf("???????:%s\n",buffer);
        send(conn, buffer, len, 0);
        printf("????????:%s\n",buffer);
    }
    close(conn);
    close(server_sockfd);
    return 0;
}
