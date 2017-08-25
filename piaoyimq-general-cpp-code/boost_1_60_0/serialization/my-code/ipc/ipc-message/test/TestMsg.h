/*
 * TestMsg.h
 *
 *  Created on: 2016年6月4日
 *      Author: root
 */

#ifndef GENERAL_IPC_IPC_MESSAGE_TEST_TESTMSG_H_
#define GENERAL_IPC_IPC_MESSAGE_TEST_TESTMSG_H_
#include "InterfaceMessage.h"

class TestMsg: public InterfaceMessage
{
public:
	explicit TestMsg();
	explicit TestMsg(const SequenceId& sequenceId, std::string& msgData);
	virtual ~TestMsg();

	virtual void serialize(boost::archive::binary_oarchive& archive) const;
	virtual void unserialize(boost::archive::binary_iarchive& archive);
	virtual void printMessage(std::ostream& os) const;

	virtual int getId() const;

	const std::string& getCmdRspData() const;

private:
	friend class boost::serialization::access;

	template<class Archive>
	inline void serialize(Archive & ar, const unsigned int /* file_version */);
	std::string msgData;
};

template<class Archive>
void TestMsg::serialize(Archive & ar, const unsigned int /* file_version */)
{
	ar & msgData;
}
#endif /* GENERAL_IPC_IPC_MESSAGE_TEST_TESTMSG_H_ */
