/*
 * ReactorImplementation.cpp
 *
 *  Created on: Dec 17, 2017
 *      Author: ezhweib
 */


#include "ReactorImplementation.h"
#include "EpollDemultiplexer.h"



#define INITSIZE 100


namespace reactor
{

ReactorImplementation::ReactorImplementation()
{
    m_demultiplexer = new EpollDemultiplexer();
    m_eventtimer = new TimeHeap(INITSIZE);
}


ReactorImplementation::~ReactorImplementation()
{
    delete m_demultiplexer;
}


int ReactorImplementation::RegisterHandler(IEventHandler * handler, event_t evt)
{
    handle_t handle = handler->GetHandle();
    std::map<handle_t, IEventHandler *>::iterator it = m_handlers.find(handle);
    if (it == m_handlers.end())
    {
        m_handlers[handle] = handler;
    }
    return m_demultiplexer->RequestEvent(handle, evt);
}


int ReactorImplementation::RemoveHandler(IEventHandler * handler)
{
    handle_t handle = handler->GetHandle();
    m_handlers.erase(handle);
    return m_demultiplexer->UnrequestEvent(handle);
}


//parm timeout is useless.
void ReactorImplementation::HandleEvents()
{
    int timeout = 0;
    if (m_eventtimer->top() == NULL)
    {
        timeout = 0;
    }
    else
    {
        timeout = ((m_eventtimer->top())->expire - time(NULL)) * 1000;
    }
    m_demultiplexer->WaitEvents(&m_handlers, timeout, m_eventtimer);
}


int ReactorImplementation::RegisterTimerTask(TimeHeap::HeapTimer* timerevent)
{
    if (timerevent == NULL)
        return -1;
    m_eventtimer->add_timer(timerevent);
    return 0;
}

}
