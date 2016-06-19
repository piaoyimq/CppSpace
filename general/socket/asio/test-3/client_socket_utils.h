/*
 * client_socket_utils.h
 *
 *  Created on: 2016年6月19日
 *      Author: piaoyimq
 */
/*客户端与服务器的逻辑也差不多，区别就是在于客户端通过connect得到socket_session，
 * 而服务器是通过accept得到socket_session。*/

#pragma once
#include "socket_session.h"
#include "session_manager.h"
#include <boost/algorithm/string.hpp>
#include <firebird/message/message.hpp>

namespace firebird
{
class FIREBIRD_DECL client_socket_utils
{
public:
	client_socket_utils();
	~client_socket_utils();

	void session_connect(std::vector<socket_session_ptr>& vSession);
	void session_connect(socket_session_ptr pSession);
	//socket_session_ptr get_session(std::string& addr);
	boost::asio::io_service& get_io_service()
	{	return m_io_srv;}

protected:
	virtual void handle_read_data(message& msg, socket_session_ptr pSession) = 0;

private:
	boost::asio::io_service m_io_srv;
	boost::asio::io_service::work m_work;
	session_manager m_manager;

	void handle_connect(const boost::system::error_code& error, tcp::resolver::iterator endpoint_iterator, socket_session_ptr pSession);

	void close_callback(socket_session_ptr session);
	void read_data_callback(const boost::system::error_code& e, socket_session_ptr session, message& msg);
};
}
