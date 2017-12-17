/*
 * IIEventHandler.h
 *
 *  Created on: Dec 17, 2017
 *      Author: ezhweib
 */

#ifndef PIAOYIMQ_GENERAL_CPP_CODE_DESIGN_PATTERN_REACTOR_REACTOR1_IEVENTHANDLER_H_
#define PIAOYIMQ_GENERAL_CPP_CODE_DESIGN_PATTERN_REACTOR_REACTOR1_IEVENTHANDLER_H_

#include "Reactor.h"




namespace reactor
{

class IEventHandler
{
public:

    virtual reactor::handle_t GetHandle() const = 0;

    virtual void HandleRead() {}

    virtual void HandleWrite() {}

    virtual void HandleError() {}

protected:

    IEventHandler() {}

    virtual ~IEventHandler() {}
};

}
#endif /* PIAOYIMQ_GENERAL_CPP_CODE_DESIGN_PATTERN_REACTOR_REACTOR1_IEVENTHANDLER_H_ */
