/*
 * ATC.hpp
 *
 *  Created on: 2016年6月17日
 *      Author: piaoyimq
 */

#ifndef GENERAL_IO_ASIO_TEST2_ATC_HPP_
#define GENERAL_IO_ASIO_TEST2_ATC_HPP_

#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <iostream>
#include <set>
#include <map>
#include "Message.hpp"
#include "SQLite.hpp"

using boost::asio::deadline_timer;
using boost::asio::ip::tcp;
using boost::asio::ip::udp;

using namespace std;
using namespace PWRD;
using namespace PWRD::SQL;
using namespace PWRD::Protocol;

typedef map<string, int> Peers;
typedef map<string, int> Stats;
typedef map<string, string> Files;
typedef map<string, tcp::socket*> Sockets;
#define DEADLINE 10
#define MAXLEN 1024
#define SECONDS 5

namespace PWRD
{
namespace Network
{
class Session
{
public:
	Session(boost::asio::ip::tcp::socket &_socket, boost::asio::io_service &io_service, const char *path);
	~Session();
	void start();

	void stop();
	void delivery(string &ip);

private:
	void handle_connect(const boost::system::error_code& ec, tcp::resolver::iterator endpoint_iter);

	void start_read();

	void handle_read_header(const boost::system::error_code& ec);

	void start_delivery();

	void handle_read_body(const boost::system::error_code& ec);

	void start_write_heartbeat();

	void handle_write_heartbeat(const boost::system::error_code& ec);
	void handle_wait(const boost::system::error_code& ec);
	void handle_write_config(const boost::system::error_code& ec, Configure &conf);

private:
	bool stopped_;
	tcp::socket &socket_;
	Header header_;
	HeartbeatMessage heartbeat_message_;
	DataMessage packet_message_;
	ConfigMessage conf_message_;
	deadline_timer heartbeat_timer_;
	Callback *callback_;
	char *input_stream_;

};

class SessionPool
{
public:
	SessionPool();
	~SessionPool();
	void join(string&, Session *);
	void delivery(string&);
	void leave(string &ip);

private:
	std::map<string, Session *> session_pool_;
};
/************************************************************
 * Usage: Client will connect to every server and it's never
 *     stopped until you shutdown it. Connect action will
 *    start in every DEADLINE seconds
 ************************************************************/
class Client
{
public:
	void stop();
	void start();
	Client(boost::asio::io_service &_io_service, boost::asio::io_service &_io_service_data, const char *_sqlite);
protected:
	void handle_wait_update();
	void handle_update();
	void start_connect();
	void handle_connect(const boost::system::error_code& ec, Peers::iterator it);
	void handle_deadline();
	void check_all();
private:

	Peers peers_;
	Sockets sockets_;
	Stats status_;
	Files files_;

	deadline_timer deadline_;
	boost::asio::io_service &io_service_;
	const char * sqlite_;
	bool all_;

	SessionPool session_pool_;

	boost::asio::io_service &io_service_data_;

};
}
}


#endif /* GENERAL_IO_ASIO_TEST2_ATC_HPP_ */
