/*
 * Message.cpp
 *
 *  Created on: 2016年6月3日
 *      Author: root
 */

#include "../include/Message.h"
#include <string>
#include <sstream>
#include <iostream>
#include <atomic>


Message::Message(const SequenceId& sequenceId) :
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
