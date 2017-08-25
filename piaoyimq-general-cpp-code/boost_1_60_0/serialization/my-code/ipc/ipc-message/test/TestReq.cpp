/*
 * TestReq.cpp
 *
 *  Created on: 2016年6月4日
 *      Author: root
 */
#include "TestReq.h"

TestReq::TestReq()
{
}

TestReq::TestReq(std::string& testReqData) :
		reqData(testReqData)
{
}

TestReq::~TestReq()
{
}

void TestReq::serialize(boost::archive::binary_oarchive& archive) const
{
	archive << *this;
}

void TestReq::unserialize(boost::archive::binary_iarchive& archive)
{
	archive >> *this;
}

void TestReq::printMessage(std::ostream& os) const
{
	os << "[";
	os << "TestReqData=";
	os << reqData;
	os << "]";
}

int TestReq::getId() const
{
	return ReqId;
}

const std::string& TestReq::getTestReqData() const
{
	return reqData;
}
