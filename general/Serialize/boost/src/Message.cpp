/*
 * Message.cpp
 *
 *  Created on: 2016年6月3日
 *      Author: root
 */

#include "Message.h"
#include <string>
#include <sstream>
#include <iostream>
#include <atomic>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/utility.hpp>


std::string Printable::toString() const
{
    std::stringstream ss;
    print(ss);

    return ss.str();
}

void SequenceId::serialize(boost::archive::binary_oarchive& archive) const
{
    	archive << *this;
}

void SequenceId::unserialize(boost::archive::binary_iarchive& archive)
{
    archive >> *this;
}



bool SequenceId::operator==(const SequenceId& sequenceId) const
{
    return value_ == sequenceId.value_;
}

bool SequenceId::operator <(const SequenceId& other) const
{
    return value_ < other.value_;
}

std::ostream& SequenceId::print(std::ostream& os) const
{
    os << value_;
    return os;
}

SequenceId SequenceId::generateNextId()
{
    // The generation must be thread safe
    // TODO: its still not fully thread safe with a static variable,
    // it can't be called from two threads at the same time the first time its called.
	std::atomic <uint32_t> processGlobalSequenceCounter(1);
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

SequenceId& SequenceId::operator=(const SequenceId& sequenceId)
{
    this->value_ = sequenceId.value_;
    return *this;
}

void Message::serialize(boost::archive::binary_oarchive& archive) const
{
	sequenceId_.serialize(archive);
	archive << *this;
}

void Message::unserialize(boost::archive::binary_iarchive& archive)
{
	sequenceId_.unserialize(archive);
	archive >> *this;
}

void Message::print(std::ostream& os) const
{
//    os << "[Message "
//        "id=" << getId() << ", "
//        "sequenceId=" << getSequenceId() << " ";
//
//    printMessage(os);     // Template method call to subclass
//
//    os << "]";
}



int main()
{
	std::ostringstream archiveStream;
	boost::archive::binary_oarchive archive(archiveStream);

	Message msg;
	Message msgb;
	msg.a = 999;
	msg.serialize(archive);

	std::cout << "__________" <<  archiveStream.str() << "-----"<<  std::endl;


	std::istringstream iss(archiveStream.str());
	boost::archive::binary_iarchive iar(iss);

	msgb.unserialize(iar);

	std::cout << "____" <<  msgb.a  << " ____" << msgb.sequenceId_.value_<< std::endl;
}
