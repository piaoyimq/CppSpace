/*
 * TestTimeServer.h
 *
 *  Created on: Dec 17, 2017
 *      Author: ezhweib
 */

#ifndef PIAOYIMQ_GENERAL_CPP_CODE_DESIGN_PATTERN_REACTOR_REACTOR1_TESTTIMESERVER_H_
#define PIAOYIMQ_GENERAL_CPP_CODE_DESIGN_PATTERN_REACTOR_REACTOR1_TESTTIMESERVER_H_


#include "Reactor.h"
#include "IEventHandler.h"
#include <cstdint>
#include <cstdio>




#define g_reactor (*(sGlobal->g_reactor_ptr))
const size_t kBufferSize = 1024;
char g_read_buffer[kBufferSize];
char g_write_buffer[kBufferSize];


namespace reactor
{

class TimeServer: public reactor::IEventHandler
{
public:
    class RequestHandler: public reactor::IEventHandler
    {
    public:

        RequestHandler(reactor::handle_t handle) :
                IEventHandler(), m_handle(handle)
        {
        }

        virtual reactor::handle_t GetHandle() const
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

        reactor::handle_t m_handle;
    };

    TimeServer(const char * ip, unsigned short port) :
            IEventHandler(), m_ip(ip), m_port(port)
    {
    }

    bool Start();

    virtual reactor::handle_t GetHandle() const
    {
        return m_handle;
    }

    virtual void HandleRead();

private:

    reactor::handle_t m_handle;

    std::string m_ip;

    uint16_t m_port;
};

}
#endif /* PIAOYIMQ_GENERAL_CPP_CODE_DESIGN_PATTERN_REACTOR_REACTOR1_TESTTIMESERVER_H_ */
