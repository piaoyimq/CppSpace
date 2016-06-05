/*
 * TestReq.h
 *
 *  Created on: 2016年6月4日
 *      Author: root
 */

#ifndef GENERAL_IPC_IPC_MESSAGE_TEST_TESTREQ_H_
#define GENERAL_IPC_IPC_MESSAGE_TEST_TESTREQ_H_

#include "InterfaceMessage.h"
class TestReq: public InterfaceMessage
{
public:
	explicit TestReq();
	explicit TestReq(std::string& testReqData);
	virtual ~TestReq();

	virtual void serialize(boost::archive::binary_oarchive& archive) const;
	virtual void unserialize(boost::archive::binary_iarchive& archive);

	virtual void printMessage(std::ostream& os) const;
	virtual int getId() const;

	const std::string& getTestReqData() const;

private:
	friend class boost::serialization::access;

	template<class Archive>
	inline void serialize(Archive & ar, const unsigned int /* file_version */);
	std::string reqData;		// it can be a self-definition class.
};

template<class Archive>
void TestReq::serialize(Archive & ar, const unsigned int /* file_version */)
{
	ar & reqData;
}

#endif /* GENERAL_IPC_IPC_MESSAGE_TEST_TESTREQ_H_ */
