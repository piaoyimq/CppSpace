/*
 * AsioTcp.h
 *
 *  Created on: Jun 15, 2016
 *      Author: piaoyimq
 */

#pragma once
#include <boost/asio.hpp>
#include <boost/make_shared.hpp>
#include <boost/enable_shared_from_this.hpp>

typedef boost::asio::ip::tcp::socket socket_t;
typedef void* socket_handle;

class INetCallback
{
public:
 virtual void OnNewConnection(socket_handle newSocket) = 0;
 virtual void OnRecvData(socket_handle socket, const char* pData, UINT32 nDataSize) = 0;
};

class CTcpSession : public boost::enable_shared_from_this
{
public:
 CTcpSession(boost::asio::io_service& ioService, INetCallback* pINetCallback);
 void   HandleRead(const boost::system::error_code& ec, size_t bytes_transferred);
 void   HandleWrite(const boost::system::error_code& ec);
 void   StartRead();
 void   SendMsg(const char* pData, UINT32 nDataSize);

 socket_t&  GetSocket() { return m_socket; }
private:
 enum { max_length = 1024 };
 char m_dataRecvBuff[max_length];
 socket_t m_socket;
 INetCallback* m_pINetCallback;
};

class CAsioTcp
{
private:
 typedef boost::shared_ptr TcpSessionPtr;
 typedef boost::asio::ip::tcp::acceptor acceptor;
public:
 CAsioTcp(INetCallback* pINetCallback);
 virtual ~CAsioTcp(void);

 void   SendMsg(socket_handle socket, const char* pData, UINT32 nDataSize);
 void   Start();
private:
 void   StartAccept();
 void   AcceptHandler(const boost::system::error_code& ec, TcpSessionPtr pTcpSession);

private:
 boost::asio::io_service m_ioservice;
 boost::shared_ptr m_pAcceptor;
 INetCallback* m_pINetCallback;
};
