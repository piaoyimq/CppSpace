/*
 * IIEventHandler.h
 *
 *  Created on: Dec 17, 2017
 *      Author: ezhweib
 */

#ifndef PIAOYIMQ_GENERAL_CPP_CODE_DESIGN_PATTERN_REACTOR_REACTOR1_IEVENTHANDLER_H_
#define PIAOYIMQ_GENERAL_CPP_CODE_DESIGN_PATTERN_REACTOR_REACTOR1_IEVENTHANDLER_H_


#include <cstdint>



namespace reactor
{

typedef int32_t handle_t;
typedef uint32_t event_t;


class IEventHandler
{
public:

    virtual handle_t GetHandle() const = 0;

    virtual void HandleRead() {}

    virtual void HandleWrite() {}

    virtual void HandleError() {}

protected:

    IEventHandler() {}

    virtual ~IEventHandler() {}
};

}
#endif /* PIAOYIMQ_GENERAL_CPP_CODE_DESIGN_PATTERN_REACTOR_REACTOR1_IEVENTHANDLER_H_ */
