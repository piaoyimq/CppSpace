/*
 * server_socket_utils.h
 *
 *  Created on: 2016年6月19日
 *      Author: piaoyimq
 */

/*
 * 服务器我定义为server_socket_utils，拥有一个session_manager，每当accept成功得到一个socket_session时，都会将其增加到session_manager去管理，注册相关回调函数。

 read_data_callback   接收到数据的回调函数

 收到数据之后，也就是数据包的body部分，反序列化出command、business_type、app_id和data（我使用到了thrift），如果command==normal正常的业务包，会调用handle_read_data传入data。

 close_callback 关闭socket_session触发的回调函数

 根据id将该连接从session_manager中删除掉
 * */

#pragma once
#include "socket_session.h"
#include "session_manager.h"
#include <boost/format.hpp>
#include <firebird/message/message.hpp>

namespace firebird
{
using boost::asio::ip::tcp;

class FIREBIRD_DECL server_socket_utils
{
private:
	boost::asio::io_service m_io_srv;
	boost::asio::io_service::work m_work;
	tcp::acceptor m_acceptor;

	void handle_accept(socket_session_ptr session, const boost::system::error_code& error);

	void close_callback(socket_session_ptr session);
	void read_data_callback(const boost::system::error_code& e,
			socket_session_ptr session, message& msg);

protected:
	virtual void handle_read_data(message& msg, socket_session_ptr pSession) = 0;

public:
	server_socket_utils(int port);
	~server_socket_utils(void);

	void start();
	boost::asio::io_service& get_io_service()
	{	return m_io_srv;}

	session_manager m_manager;
};
}
