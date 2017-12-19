/*
 * Reactor.cpp
 *
 *  Created on: Dec 17, 2017
 *      Author: ezhweib
 */


#include "Reactor.h"




namespace reactor
{

Reactor::Reactor()
{
    m_reactor_impl = new ReactorImplementation();
}

Reactor::~Reactor()
{
    delete m_reactor_impl;
}

int Reactor::RegisterHandler(IEventHandler * handler, event_t evt)
{
    return m_reactor_impl->RegisterHandler(handler, evt);
}

int Reactor::RemoveHandler(IEventHandler * handler)
{
    return m_reactor_impl->RemoveHandler(handler);
}

void Reactor::HandleEvents()
{
    m_reactor_impl->HandleEvents();
}

int Reactor::RegisterTimerTask(TimeHeap::HeapTimer* timerevent)
{
    return m_reactor_impl->RegisterTimerTask(timerevent);
}

}
