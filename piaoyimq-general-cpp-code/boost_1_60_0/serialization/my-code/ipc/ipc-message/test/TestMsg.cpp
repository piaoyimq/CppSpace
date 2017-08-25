/*
 * TestMsg.cpp
 *
 *  Created on: 2016年6月4日
 *      Author: root
 */
#include "TestMsg.h"

TestMsg::TestMsg()
{
}

TestMsg::TestMsg(const SequenceId& sequenceId, std::string& msgData) :
		InterfaceMessage(sequenceId), msgData(msgData)
{
}

TestMsg::~TestMsg()
{
}

void TestMsg::serialize(boost::archive::binary_oarchive& archive) const
{
	archive << *this;
}

void TestMsg::unserialize(boost::archive::binary_iarchive& archive)
{
	archive >> *this;
}

void TestMsg::printMessage(std::ostream& os) const
{
	os << "[CmdRspData=";
	os << msgData;
	os << "]";
}

int TestMsg::getId() const
{
	return MsgId;
}

const std::string& TestMsg::getCmdRspData() const
{
	return msgData;

}
