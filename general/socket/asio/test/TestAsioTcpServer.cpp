/*
 * TestAsioTcpServer.cpp
 *
 *  Created on: Jun 15, 2016
 *      Author: piaoyimq
 */

#include "stdafx.h"
#include <iostream>
#include "AsioTcp.h"
#include <boost/make_shared.hpp>

class CTestAsioTcpServer : public INetCallback
{

public:
 CTestAsioTcpServer()
 {
  m_pAsioTcp = boost::make_shared(this);
  m_pAsioTcp->Start();
 }
 void Run()
 {
  while (true)
  {
   Sleep(10);
  }
 }
 virtual void OnNewConnection( socket_handle newSocket )
 {
  std::cout << "OnNewConnection" << std::endl;
 }

 virtual void OnRecvData( socket_handle socket, const char* pData, UINT32 nDataSize )
 {
  std::cout << "OnRecvData:" << pData  << " size=" << nDataSize << std::endl;

  m_pAsioTcp->SendMsg(socket, "echo", 4);

 }

private:
 boost::shared_ptr m_pAsioTcp;
};

int _tmain(int argc, _TCHAR* argv[])
{
 CTestAsioTcpServer tcpSrv;
 tcpSrv.Run();

 return 0;
}


