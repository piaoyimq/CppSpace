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
#include "TimeHeap.h"


namespace reactor
{

class EpollDemultiplexer: public IEventDemultiplexer
{
public:
    EpollDemultiplexer();

    virtual ~EpollDemultiplexer();

    virtual int WaitEvents(std::map<handle_t, IEventHandler*> * handlers, int timeout = 0, TimeHeap* event_timer = nullptr);

    virtual int RequestEvent(handle_t handle, event_t evt);

    virtual int UnrequestEvent(handle_t handle);

private:

    int m_epoll_fd;
    int m_fd_num;
};

}

#endif /* PIAOYIMQ_GENERAL_CPP_CODE_DESIGN_PATTERN_REACTOR_REACTOR1_EPOLLDEMULTIPLEXER_H_ */
