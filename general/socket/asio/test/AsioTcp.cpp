/*
 * AsioTcp.cpp
 *
 *  Created on: Jun 15, 2016
 *      Author: piaoyimq
 */

//#include "StdAfx.h"
#include "AsioTcp.h"
#include <boost/thread.hpp>

using namespace boost::asio;

CAsioTcp::CAsioTcp(INetCallback* pINetCallback)
 : m_pINetCallback(pINetCallback)
{
 m_pAcceptor = boost::make_shared(m_ioservice, ip::tcp::endpoint(ip::tcp::v4(), 1688));
 StartAccept();
}


CAsioTcp::~CAsioTcp(void)
{
}

void CAsioTcp::StartAccept()
{
 TcpSessionPtr pTcpSession = boost::make_shared(m_ioservice, m_pINetCallback);
 m_pAcceptor->async_accept(pTcpSession->GetSocket(), boost::bind(&CAsioTcp::AcceptHandler, this, boost::asio::placeholders::error, pTcpSession));
}

void CAsioTcp::AcceptHandler( const boost::system::error_code& ec, TcpSessionPtr pTcpSession )
{
 if (ec)
 {
  return;
 }

 if (m_pINetCallback != NULL)
 {
  m_pINetCallback->OnNewConnection(&pTcpSession);
 }

 StartAccept();
 pTcpSession->StartRead();
}

void CAsioTcp::SendMsg( socket_handle socket, const char* pData, UINT32 nDataSize )
{
 TcpSessionPtr* ppTcpSession = reinterpret_cast(socket);
 (*ppTcpSession)->SendMsg(pData, nDataSize);
}

void CAsioTcp::Start()
{
 m_ioservice.run();
}

CTcpSession::CTcpSession(boost::asio::io_service& ioService, INetCallback* pINetCallback)
 : m_socket(ioService)
 , m_pINetCallback(pINetCallback)
{

}

void CTcpSession::HandleRead( const boost::system::error_code& ec, size_t bytes_transferred )
{
 if (!ec)
 {
  if (m_pINetCallback != NULL)
  {
   m_pINetCallback->OnRecvData(this, m_dataRecvBuff, bytes_transferred);
  }
  StartRead();
 }
}

void CTcpSession::StartRead()
{
 m_socket.async_read_some(boost::asio::buffer(m_dataRecvBuff, max_length),
  boost::bind(&CTcpSession::HandleRead, shared_from_this(),
  boost::asio::placeholders::error,
  boost::asio::placeholders::bytes_transferred));
}

void CTcpSession::SendMsg( const char* pData, UINT32 nDataSize )
{
 boost::asio::async_write(m_socket,
  boost::asio::buffer(pData, nDataSize),
  boost::bind(&CTcpSession::HandleWrite, shared_from_this(),
  boost::asio::placeholders::error));

}

void CTcpSession::HandleWrite( const boost::system::error_code& ec )
{

}


