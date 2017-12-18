/*
 * TestTimeServer.cpp
 *
 *  Created on: Dec 17, 2017
 *      Author: ezhweib
 */


#include "TestTimeServer.h"
#include "test-common.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

namespace reactor
{

const size_t kBufferSize = 1024;
char g_read_buffer[kBufferSize];
char g_write_buffer[kBufferSize];

void TestTimeServer::RequestHandler::HandleWrite()
{
    struct tm *ttime;
    char now[64];
    time_t tt;

    memset(now, 0, 64);
    tt = time(NULL);
    ttime = localtime(&tt);
    strftime(now, 64, "%Y-%m-%d %H:%M:%S", ttime);

    memset(g_write_buffer, 0, sizeof(g_write_buffer));
    int len = sprintf(g_write_buffer, "current time: %s\r\n", now);
    len = send(m_handle, g_write_buffer, len, 0);
    if (len > 0)
    {
        fprintf(stderr, "send response to client, fd=%d\n", (int) m_handle);
        g_reactor.RegisterHandler(this, kReadEvent);
    }
    else
    {
        ReportSocketError("send");
    }
}

void TestTimeServer::RequestHandler::HandleRead()
{
    memset(g_read_buffer, 0, sizeof(g_read_buffer));
    int len = recv(m_handle, g_read_buffer, kBufferSize, 0);
    if (len > 0)
    {
        if (strncasecmp("time", g_read_buffer, 4) == 0)
        {
            g_reactor.RegisterHandler(this, kWriteEvent);
        }
        else
            if (strncasecmp("exit", g_read_buffer, 4) == 0)
            {
                close (m_handle);
                g_reactor.RemoveHandler(this);
                delete this;
            }
            else
            {
                fprintf(stderr, "Invalid request: %s", g_read_buffer);
                close (m_handle);
                g_reactor.RemoveHandler(this);
                delete this;
            }
    }
    else
    {
        ReportSocketError("recv");
    }
}


bool TestTimeServer::Start()
{
    m_handle = socket(AF_INET, SOCK_STREAM, 0);
    if (!IsValidHandle(m_handle))
    {
        ReportSocketError("socket");
        return false;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(m_port);
    addr.sin_addr.s_addr = inet_addr(m_ip.c_str());
    if (bind(m_handle, (struct sockaddr *) &addr, sizeof(addr)) < 0)
    {
        ReportSocketError("bind");
        return false;
    }

    if (listen(m_handle, 10) < 0)
    {
        ReportSocketError("listen");
        return false;
    }
    return true;
}

void TestTimeServer::HandleRead()
{
    struct sockaddr addr;
    socklen_t addrlen = sizeof(addr);
    handle_t handle = accept(m_handle, &addr, &addrlen);
    if (!IsValidHandle(handle))
    {
        ReportSocketError("accept");
    }
    else
    {
        RequestHandler* handler = new RequestHandler(handle);
        if (g_reactor.RegisterHandler(handler, kReadEvent) != 0)
        {
            fprintf(stderr, "error: register handler failed\n");
            delete handler;
        }
    }
}

}
