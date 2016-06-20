/*
 * socket_session.h
 *
 *  Created on: 2016年6月19日
 *      Author: piaoyimq
 */
/*
 * Tag(3bytes)|Length(4bytes) || Command(2bytes)|Business_type(2bytes)|App_id(2bytes)|Data
 Head                     ||                    Body
 Tag：检查数据包是否合法，具体会在下面讲解；

 Length：描述Body的长度；

 Command：表示数据包的类型，0表示心跳包（长连接需要心跳来检测连接是否正常），1表示注册包（客户端连接上服务器之后要将相关信息注册给服务器），2表示业务消息包；

 business_type：业务消息包类型，服务器会根据业务消息包类型将数据路由到对应的客户端（客户端是有业务类型分类的）；

 app_id：客户端唯一标识符；

 Data：消息数据；
 *
 *
 *
 *
 * 这里注意的是，定义了一个tag="KDS"，目的是为了检查收到的数据包是否有效，
 * 每一个数据包前3个字节不为“KDS”，那么就认为是非法的请求包，你也可以定义tag等于其它字符串，
 * 只要按协议发包就正常，当然这是比较简单的数据包检查方法了。比较严谨的方法是双方使用哈希算法来检查的，
 * 怎么做，这里先不做详解。
 *
 *
 * */

#pragma once
#include <iostream>
#include <list>
#include <hash_map>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <firebird/log/logger_log4.hpp>
#include <firebird/detail/config.hpp>
#include <firebird/socket_utils/message_archive.hpp>

using boost::asio::ip::tcp;

namespace firebird
{
enum command
{
	heartbeat = 0, regist, normal
};

const std::string tag = "KDS";

class FIREBIRD_DECL socket_session;
typedef boost::shared_ptr<socket_session> socket_session_ptr;

class FIREBIRD_DECL socket_session:
public boost::enable_shared_from_this<socket_session>,
private boost::noncopyable
{
public:
	typedef boost::function<void(socket_session_ptr)> close_callback;
	typedef boost::function<void(
			const boost::system::error_code&,
			socket_session_ptr, message&)> read_data_callback;

	socket_session(boost::asio::io_service& io_service);
	~socket_session(void);

	DWORD id()
	{	return m_id;}
	WORD get_business_type()
	{	return m_business_type;}
	void set_business_type(WORD type)
	{	m_business_type = type;}
	DWORD get_app_id()
	{	return m_app_id;}
	void set_app_id(DWORD app_id)
	{	m_app_id = app_id;}
	std::string& get_remote_addr()
	{	return m_name;}
	void set_remote_addr(std::string& name)
	{	m_name = name;}
	tcp::socket& socket()
	{	return m_socket;}

	void installCloseCallBack(close_callback cb)
	{	close_cb = cb;}
	void installReadDataCallBack(read_data_callback cb)
	{	read_data_cb = cb;}

	void start();
	void close();
	void async_write(const std::string& sMsg);
	void async_write(message& msg);

	bool is_timeout();
	void set_op_time()
	{	std::time(&m_last_op_time);}

private:
	static boost::detail::atomic_count m_last_id;

	DWORD m_id;
	WORD m_business_type;
	DWORD m_app_id;
	std::string m_name;
	boost::array<char, 7> sHeader;
	std::string sBody;

	tcp::socket m_socket;
	boost::asio::io_service& m_io_service;

	std::time_t m_last_op_time;

	close_callback close_cb;
	read_data_callback read_data_cb;

	//发送消息
	void handle_write(const boost::system::error_code& e,
			std::size_t bytes_transferred, std::string* pmsg);

	//读消息头
	void handle_read_header(const boost::system::error_code& error);
	//读消息体
	void handle_read_body(const boost::system::error_code& error);

	void handle_close();
};
}

