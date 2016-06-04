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

Message::Message(const SequenceId& sequenceId = SequenceId::generateNextId()) :
		sequenceId_(sequenceId)
{
}

Message::~Message()
{
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
	os << "[Message "
			"id=" << getId() << ", "
			"sequenceId=" << getSequenceId() << " ";

	printMessage(os);     // Template method call to subclass

	os << "]";
}
