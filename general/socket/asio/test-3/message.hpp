/*
 * message_archive.hpp
 *
 *  Created on: 2016年6月19日
 *      Author: piaoyimq
 */

#pragma once
#include "socket_session.h"

typedef unsigned long DWORD;
typedef unsigned short WORD;

#define LOG4CXX_ERROR(a, b)  std::cout << b
#define LOG4CXX_DEBUG(a, b)	std::cout << b
#define LOG4CXX_FATAL(a, b)		std::cout << b

enum Command
{
	heartbeat = 0, regist, normal
};

class message
{
public:
	Command command;
	WORD business_type;
	DWORD app_id;
	std::string _data;
	std::string& data()
	{
		return _data;
	}
};
