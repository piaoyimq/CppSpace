/*
 * message_archive.hpp
 *
 *  Created on: 2016年6月19日
 *      Author: piaoyimq
 */

#pragma once
#include <boost/shared_ptr.hpp>
#include <transport/TBufferTransports.h>
#include <protocol/TProtocol.h>
#include <protocol/TBinaryProtocol.h>

namespace firebird
{
using namespace apache::thrift;
using namespace apache::thrift::transport;
using namespace apache::thrift::protocol;

template<typename T>
void thrift_iserialize(T& stu, std::string& s)
{
	boost::shared_ptr < TMemoryBuffer > trans(new TMemoryBuffer((uint8_t*) &s[0], s.size()));
	boost::shared_ptr<TProtocol> proto(new TBinaryProtocol(trans));
	stu.read(proto.get());
}

template<typename T>
void thrift_oserialize(T& stu, std::string& s)
{
	boost::shared_ptr < TMemoryBuffer > trans(new TMemoryBuffer());
	boost::shared_ptr<TProtocol> proto(new TBinaryProtocol(trans));
	stu.write(proto.get());
	s = trans->getBufferAsString();
}
}

