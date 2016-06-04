/*
 * TestRsp.h
 *
 *  Created on: 2016年6月4日
 *      Author: root
 */

#ifndef GENERAL_IPC_IPC_MESSAGE_TEST_TESTRSP_H_
#define GENERAL_IPC_IPC_MESSAGE_TEST_TESTRSP_H_
#include "InterfaceMessage.h"

class TestRsp: public InterfaceMessage
{
public:
	explicit TestRsp();
	explicit TestRsp(const SequenceId& sequenceId, bool success);
	virtual ~TestRsp();

	virtual void serialize(boost::archive::binary_oarchive& archive) const;
	virtual void unserialize(boost::archive::binary_iarchive& archive);

	virtual void printMessage(std::ostream& os) const;
	virtual int getId() const;

	bool isSuccess() const;

private:
	friend class boost::serialization::access;

	template<class Archive>
	inline void serialize(Archive & ar, const unsigned int /* file_version */);
	bool success_;
};

template<class Archive>
void TestRsp::serialize(Archive & ar, const unsigned int /* file_version */)
{
	ar & success_;
}
#endif /* GENERAL_IPC_IPC_MESSAGE_TEST_TESTRSP_H_ */
