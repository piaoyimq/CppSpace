/*
 * InterfaceMessage.cpp
 *
 *  Created on: 2016年6月4日
 *      Author: root
 */
#include "InterfaceMessage.h"

//const ResourceId InterfaceMessage::Id("LocalCmdDispatchingIf", "R1.0");

InterfaceMessage::~InterfaceMessage()
{
}

InterfaceMessage::InterfaceMessage(const SequenceId& sequenceId) :
		Message(sequenceId)
{
}

InterfaceMessage::InterfaceMessage()
{
}
