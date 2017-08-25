/*
 * InterfaceMessage.h
 *
 *  Created on: 2016年6月4日
 *      Author: root
 */

#ifndef GENERAL_IPC_IPC_MESSAGE_TEST_INTERFACEMESSAGE_H_
#define GENERAL_IPC_IPC_MESSAGE_TEST_INTERFACEMESSAGE_H_

#include "../include/Message.h"

//we can inherit Message to InterfaceMessage in different namespace, here only a example

class InterfaceMessage : public Message
{
public:

    enum MessageIds
    {
        ReqId = Message::UnreservedMessageIdStart,
        MsgId,
        RspId
    };

    ~InterfaceMessage();

    explicit InterfaceMessage(const SequenceId& sequenceId);

    explicit InterfaceMessage();


//    static const ResourceId Id;
};

#endif /* GENERAL_IPC_IPC_MESSAGE_TEST_INTERFACEMESSAGE_H_ */
