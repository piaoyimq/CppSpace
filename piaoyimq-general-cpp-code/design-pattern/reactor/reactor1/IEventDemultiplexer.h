/*
 * IEventDemultiplexer.h
 *
 *  Created on: Dec 17, 2017
 *      Author: ezhweib
 */

#ifndef PIAOYIMQ_GENERAL_CPP_CODE_DESIGN_PATTERN_REACTOR_REACTOR1_IEVENTDEMULTIPLEXER_H_
#define PIAOYIMQ_GENERAL_CPP_CODE_DESIGN_PATTERN_REACTOR_REACTOR1_IEVENTDEMULTIPLEXER_H_

#include "Reactor.h"
#include "TimeHeap.h"
#include <map>

namespace reactor
{

class IEventDemultiplexer
{
public:

    virtual ~IEventDemultiplexer(){}

    virtual int WaitEvents(std::map<reactor::handle_t, IEventHandler*>* handlers, int timeout = 0, TimeHeap* event_timer = nullptr) = 0;

    virtual int RequestEvent(reactor::handle_t handle, reactor::event_t evt) = 0;

    virtual int UnrequestEvent(reactor::handle_t handle) = 0;
};

}
#endif /* PIAOYIMQ_GENERAL_CPP_CODE_DESIGN_PATTERN_REACTOR_REACTOR1_IEVENTDEMULTIPLEXER_H_ */
