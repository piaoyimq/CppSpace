/*
 * Reactor.h
 *
 *  Created on: Dec 17, 2017
 *      Author: ezhweib
 */


#ifndef PIAOYIMQ_GENERAL_CPP_CODE_DESIGN_PATTERN_REACTOR_REACTOR1_REACTOR_H_
#define PIAOYIMQ_GENERAL_CPP_CODE_DESIGN_PATTERN_REACTOR_REACTOR1_REACTOR_H_


#include "IEventHandler.h"
#include "ReactorImplementation.h"
#include <cstdint>




namespace reactor
{

typedef uint32_t event_t;

enum
{
    kReadEvent    = 0x01,
    kWriteEvent   = 0x02,
    kErrorEvent   = 0x04,
    kEventMask    = 0xff
};

typedef int32_t reactor::handle_t;


class Reactor
{
public:

    Reactor();

    ~Reactor();

    int RegisterHandler(IEventHandler * handler, event_t evt);

    int RemoveHandler(IEventHandler * handler);

    void HandleEvents();

    int RegisterTimerTask(HeapTimer* timerevent);

private:

    Reactor(const Reactor &);

    Reactor & operator=(const Reactor &);

private:

    ReactorImplementation * m_reactor_impl;
};

}
#endif /* PIAOYIMQ_GENERAL_CPP_CODE_DESIGN_PATTERN_REACTOR_REACTOR1_REACTOR_H_ */
