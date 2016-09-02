/*
 * TestAsioTcpClient.cpp
 *
 *  Created on: Jun 15, 2016
 *      Author: piaoyimq
 */


#include "stdafx.h"
#include <boost/asio.hpp>
#include <vector>

using namespace boost::asio;

int _tmain(int argc, _TCHAR* argv[])
{
 io_service ioservice;
 ip::tcp::socket sock(ioservice);
 ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 1688);
 ioservice.run();
 try
 {
  sock.connect(ep);

  while (true)
  {
   char szData[] = "hello";
   std::cout << "send:" << szData << std::endl;
   sock.send(buffer(szData));

   Sleep(1000);
   std::vector<char> str(1024, 0);
   int nSize = sock.read_some(buffer(str));

   std::cout << "Recv from " << sock.remote_endpoint().address() << ":" << &str[0]  << " size=" << nSize << std::endl;
  }
 }
 catch (std::exception& e)
 {
  std::cout << e.what() << std::endl;
 }



 return 0;
}

