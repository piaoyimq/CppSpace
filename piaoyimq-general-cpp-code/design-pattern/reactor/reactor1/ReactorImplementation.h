/*
 * ReactorImplementation.h
 *
 *  Created on: Dec 17, 2017
 *      Author: ezhweib
 */


#ifndef PIAOYIMQ_GENERAL_CPP_CODE_DESIGN_PATTERN_REACTOR_REACTOR1_REACTORIMPLEMENTATION_H_
#define PIAOYIMQ_GENERAL_CPP_CODE_DESIGN_PATTERN_REACTOR_REACTOR1_REACTORIMPLEMENTATION_H_


#include "IEventDemultiplexer.h"
#include "TimeHeap.h"
#include <map>


namespace reactor
{

class ReactorImplementation
{
public:

    ReactorImplementation();

    ~ReactorImplementation();

    int RegisterHandler(IEventHandler * handler, event_t evt);

    int RemoveHandler(IEventHandler * handler);

    void HandleEvents();

    int RegisterTimerTask(TimeHeap::HeapTimer* timerevent);

private:

    IEventDemultiplexer* m_demultiplexer;

    std::map<handle_t, IEventHandler*> m_handlers;

    TimeHeap* m_eventtimer;
};

}

#endif /* PIAOYIMQ_GENERAL_CPP_CODE_DESIGN_PATTERN_REACTOR_REACTOR1_REACTORIMPLEMENTATION_H_ */
