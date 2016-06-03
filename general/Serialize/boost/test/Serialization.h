/*
 * serialize.h
 *
 *  Created on: 2016年5月28日
 *      Author: piaoyimq
 */

#ifndef SERIALIZE_SRC_SERIALIZATION_H_
#define SERIALIZE_SRC_SERIALIZATION_H_

#include <string>
#include <sstream>
#include "Message.h"
class Serialization
{
public:
	static std::string Serialize(const Message& msg);
	static Message DeSerialize(const std::string& message);
};


std::string Serialization::Serialize(const Message& msg)
{
	std::ostringstream archiveStream;
	boost::archive::text_oarchive archive(archiveStream);
	archive << msg;
	return archiveStream.str();
}

Message Serialization::DeSerialize(conststd::string& message)
{
	Message msg;
	std::istringstream archiveStream(message);
	boost::archive::text_iarchive archive(archiveStream);
	archive >> msg;
	return msg;
}

#endif /* SERIALIZE_SRC_SERIALIZATION_H_ */
