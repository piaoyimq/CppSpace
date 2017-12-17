/*
 * TestTimeClient.h
 *
 *  Created on: Dec 17, 2017
 *      Author: ezhweib
 */

#ifndef PIAOYIMQ_GENERAL_CPP_CODE_DESIGN_PATTERN_REACTOR_REACTOR1_TESTTIMECLIENT_H_
#define PIAOYIMQ_GENERAL_CPP_CODE_DESIGN_PATTERN_REACTOR_REACTOR1_TESTTIMECLIENT_H_

#include "Reactor.h"
#include "IEventHandler.h"
#include "test-common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>




reactor::Reactor g_reactor;

class TestTimeClient : public reactor::IEventHandler
{
public:

    TestTimeClient():IEventHandler()
    {
        m_handle = socket(AF_INET, SOCK_STREAM, 0);
        assert(IsValidHandle(m_handle));
    }

    ~TestTimeClient()
    {
        close(m_handle);
    }

    bool ConnectServer(const char * ip, unsigned short port);

    virtual reactor::handle_t GetHandle() const
    {
        return m_handle;
    }

    virtual void HandleRead();

    virtual void HandleWrite();

    virtual void HandleError()
    {
        fprintf(stderr, "server closed\n");
        close(m_handle);
        exit(0);
    }

private:

    reactor::handle_t  m_handle;
};


#endif /* PIAOYIMQ_GENERAL_CPP_CODE_DESIGN_PATTERN_REACTOR_REACTOR1_TESTTIMECLIENT_H_ */
