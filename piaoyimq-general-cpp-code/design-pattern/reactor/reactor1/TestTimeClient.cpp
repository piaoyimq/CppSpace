/*
 * TestTimeClient.cpp
 *
 *  Created on: Dec 17, 2017
 *      Author: ezhweib
 */




#include "TestTimeClient.h"




namespace reactor
{

bool TestTimeClient::ConnectServer(const char * ip, unsigned short port)
{
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);
    if (connect(m_handle, (struct sockaddr *) &addr, sizeof(addr)) < 0)
    {
        ReportSocketError("connect");
        return false;
    }
    return true;
}

virtual void TestTimeClient::HandleRead()
{
    memset(g_read_buffer, 0, kBufferSize);
    int len = recv(m_handle, g_read_buffer, kBufferSize, 0);
    if (len > 0)
    {
        fprintf(stderr, "%s", g_read_buffer);
        g_reactor.RegisterHandler(this, reactor::kWriteEvent);
    }
    else
    {
        ReportSocketError("recv");
    }
}

virtual void TestTimeClient::HandleWrite()
{
    memset(g_write_buffer, 0, kBufferSize);
    int len = sprintf(g_write_buffer, "time\r\n");
    len = send(m_handle, g_write_buffer, len, 0);
    if (len > 0)
    {
        fprintf(stderr, "%s", g_write_buffer);
        g_reactor.RegisterHandler(this, reactor::kReadEvent);
    }
    else
    {
        ReportSocketError("send");
    }
}

}
