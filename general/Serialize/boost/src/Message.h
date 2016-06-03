/*
 * test2.cpp
 *
 *  Created on: 2016年5月28日
 *      Author: piaoyimq
 */

#ifndef MESSAGE_H
#define MESSAGE_H
#include <string>
#include <boost/archive/binary_oarchive.hpp> //serialization
#include <boost/archive/binary_iarchive.hpp> //deserialization



class Printable
{
public:

    virtual ~Printable() {}

    // Print the contents of the class into the specified ostream
    virtual void print(std::ostream& os) const = 0;

    std::string toString() const;
};

class SequenceId
{
public:
	explicit SequenceId() : value_(0) {}
	explicit SequenceId(uint32_t value) : value_(value) {}
	SequenceId(const SequenceId& sequenceId) : value_(sequenceId.value_) {}
    virtual ~SequenceId() {}

    virtual void serialize(boost::archive::binary_oarchive& archive) const;
    virtual void unserialize(boost::archive::binary_iarchive& archive);

    SequenceId& operator=(const SequenceId& sequenceId);
    bool operator==(const SequenceId& sequenceId) const;
    bool operator <(const SequenceId& other) const;

    std::ostream& print(std::ostream& os) const;

    static SequenceId generateNextId();
    static const SequenceId& null();
    uint32_t value_;
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int /* file_version */)
	{
		ar & value_;
	}
};


class Message: public Printable
{
public:
	explicit Message(const SequenceId& sequenceId = SequenceId::generateNextId()) : sequenceId_(sequenceId) {}
	virtual ~Message() {}

	virtual void serialize(boost::archive::binary_oarchive& archive) const;
	virtual void unserialize(boost::archive::binary_iarchive& archive);

//	virtual int getId() const = 0;
	inline const SequenceId& getSequenceId() const;

	void print(std::ostream& os) const;

	const static int UnreservedMessageIdStart = 100;
	SequenceId sequenceId_;
	int a;

protected:
//	virtual void printMessage(std::ostream& os) const = 0;
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int /* file_version */)
	{
//		ar & UnreservedMessageIdStart & sequenceId_;
		ar & sequenceId_ & a;
	}
};

inline const SequenceId& Message::getSequenceId() const
{
	return sequenceId_;
}
#endif

