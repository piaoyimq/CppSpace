/*
 * EpollDemultiplexer.h
 *
 *  Created on: Dec 17, 2017
 *      Author: ezhweib
 */

#ifndef PIAOYIMQ_GENERAL_CPP_CODE_DESIGN_PATTERN_REACTOR_REACTOR1_EPOLLDEMULTIPLEXER_H_
#define PIAOYIMQ_GENERAL_CPP_CODE_DESIGN_PATTERN_REACTOR_REACTOR1_EPOLLDEMULTIPLEXER_H_


#include <set>
#include "IEventDemultiplexer.h"
#include "Reactor.h"


namespace reactor
{

class EpollDemultiplexer: public IEventDemultiplexer
{
public:
    EpollDemultiplexer();

    ~EpollDemultiplexer();

    virtual int WaitEvents(std::map<reactor::handle_t, EventHandler *> * handlers, int timeout = 0, time_heap* event_timer = NULL);

    virtual int RequestEvent(reactor::handle_t handle, event_t evt);

    virtual int UnrequestEvent(reactor::handle_t handle);

private:

    int m_epoll_fd;
    int m_fd_num;
};

}

#endif /* PIAOYIMQ_GENERAL_CPP_CODE_DESIGN_PATTERN_REACTOR_REACTOR1_EPOLLDEMULTIPLEXER_H_ */
