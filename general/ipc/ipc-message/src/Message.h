/*
 * Message.h
 *
 *  Created on: 2016年5月28日
 *      Author: piaoyimq
 */

#ifndef MESSAGE_H
#define MESSAGE_H
#include <string>
#include <boost/archive/binary_oarchive.hpp> //serialization
#include <boost/archive/binary_iarchive.hpp> //deserialization

class Message: public PrintOut
{
public:
	explicit Message(const SequenceId& sequenceId = SequenceId::generateNextId());
	virtual ~Message();

	virtual void serialize(boost::archive::binary_oarchive& archive) const;
	virtual void unserialize(boost::archive::binary_iarchive& archive);

	virtual int getId() const = 0;
	inline const SequenceId& getSequenceId() const;

	void print(std::ostream& os) const;

	const static int UnreservedMessageIdStart = 100;
	SequenceId sequenceId_;

protected:
	virtual void printMessage(std::ostream& os) const = 0;
	friend class boost::serialization::access;

	template<class Archive>
	inline void serialize(Archive & ar, const unsigned int /* file_version */);
};

inline const SequenceId& Message::getSequenceId() const
{
	return sequenceId_;
}

template<class Archive>
void Message::serialize(Archive & ar, const unsigned int /* file_version */)
{
	ar & sequenceId_;
}
#endif

