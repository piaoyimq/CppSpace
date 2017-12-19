/*
 * TestTimeServer.h
 *
 *  Created on: Dec 17, 2017
 *      Author: ezhweib
 */

#ifndef PIAOYIMQ_GENERAL_CPP_CODE_DESIGN_PATTERN_REACTOR_REACTOR1_TESTTIMESERVER_H_
#define PIAOYIMQ_GENERAL_CPP_CODE_DESIGN_PATTERN_REACTOR_REACTOR1_TESTTIMESERVER_H_

#include "Reactor.h"
#include "global.h"
#include "IEventHandler.h"
#include <cstdint>
#include <cstdio>
#include <unistd.h>
#include <string.h>





namespace reactor
{

class TestTimeServer: public IEventHandler
{
public:
    class RequestHandler: public IEventHandler
    {
    public:

        RequestHandler(handle_t handle) :
                m_handle(handle)
        {
        }

        virtual handle_t GetHandle() const
        {
            return m_handle;
        }

        virtual void HandleWrite();

        virtual void HandleRead();

        virtual void HandleError()
        {
            fprintf(stderr, "client %d closed\n", m_handle);
            close(m_handle);
            g_reactor.RemoveHandler(this);
            delete this;
        }

    private:

        handle_t m_handle;
    };

    TestTimeServer(const char * ip, unsigned short port) :
            m_ip(ip), m_port(port)
    {
    }

    bool Start();

    virtual handle_t GetHandle() const
    {
        return m_handle;
    }

    virtual void HandleRead();

private:

    handle_t m_handle;

    std::string m_ip;

    uint16_t m_port;
};

}
#endif /* PIAOYIMQ_GENERAL_CPP_CODE_DESIGN_PATTERN_REACTOR_REACTOR1_TESTTIMESERVER_H_ */
