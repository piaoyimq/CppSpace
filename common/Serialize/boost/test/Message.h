/*
 * test2.cpp
 *
 *  Created on: 2016年5月28日
 *      Author: piaoyimq
 */

#ifndef MESSAGE_H
#defineMESSAGE_H
#include <string>
//serialization
#include <boost/archive/text_oarchive.hpp>
//deserialization
#include <boost/archive/text_iarchive.hpp>
using std::string;
//message type
enum MessageOption
{
	Login, Logout, SendToAll, PrivateChat
};

struct Message
{
	friend class boost::serialization::access; //use this function to serialize and deserialize object
	template<class Archive> void serialize(Archive & ar, const unsigned intversion)
	{
		ar & sender & receiver & message & option;
	}
	string sender;
	string receiver;
	string message;
	MessageOption option;
};
#endif

