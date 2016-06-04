/*
 * TestRsp.cpp
 *
 *  Created on: 2016年6月4日
 *      Author: root
 */
#include "TestRsp.h"

TestRsp::TestRsp() :
		success_(false)
{
}

TestRsp::TestRsp(const SequenceId& sequenceId, bool success) :
		InterfaceMessage(sequenceId), success_(success)
{
}

TestRsp::~TestRsp()
{
}

void TestRsp::serialize(boost::archive::binary_oarchive& archive) const
{
	archive << *this;
}

void TestRsp::unserialize(boost::archive::binary_iarchive& archive)
{
	archive >> *this;
}

void TestRsp::printMessage(std::ostream& os) const
{
	os << "[success=";
	os << success_;
	os << "]";
}

int TestRsp::getId() const
{
	return InterfaceMessage::RspId;
}

bool TestRsp::isSuccess() const
{
	return success_;
}
