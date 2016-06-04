/*
 * SequenceId.cpp
 *
 *  Created on: 2016年6月4日
 *      Author: root
 */
#include "SequenceId.h"
#include <atomic>


SequenceId::SequenceId() :
		value_(0)
{
}

SequenceId::SequenceId(uint32_t value) :
		value_(value)
{
}

SequenceId::SequenceId(const SequenceId& sequenceId) :
		value_(sequenceId.value_)
//		Io::ISerializable(), value_(sequenceId.value_)
{
}

SequenceId::~SequenceId()
{
}

void SequenceId::serialize(boost::archive::binary_oarchive& archive) const
{
	archive << *this;
}

void SequenceId::unserialize(boost::archive::binary_iarchive& archive)
{
	archive >> *this;
}

void SequenceId::print(std::ostream& os) const
{
	os << value_;
}

bool SequenceId::operator==(const SequenceId& sequenceId) const
{
	return value_ == sequenceId.value_;
}

bool SequenceId::operator <(const SequenceId& other) const
{
	return value_ < other.value_;
}

SequenceId& SequenceId::operator=(const SequenceId& sequenceId)
{
	this->value_ = sequenceId.value_;
	return *this;
}

SequenceId SequenceId::generateNextId()
{
	// The generation must be thread safe
	// TODO: its still not fully thread safe with a static variable,
	// it can't be called from two threads at the same time the first time its called.
	static std::atomic<uint32_t> processGlobalSequenceCounter(1);

	uint32_t value;
	do
	{
		value = processGlobalSequenceCounter++;
		// Protect against wrap-around, 0 is not a valid sequence number
	} while (value == 0);

	return SequenceId(value);
}

const SequenceId& SequenceId::null()
{
	static const SequenceId sequenceId(0);
	return sequenceId;
}
