/*
 * TcpServer.cpp
 *
 *  Created on: Jun 15, 2016
 *      Author: piaoyimq
 */

/*
 * boost.asio给出了TCP Server的类，但是应用在数据处理中，有以下的几个问题：

1.读例程中，处理到达数据是硬编码的

2.例程中，针对每个读取的数据，分析后作了回应。

在应用中，经常需要对收到的数据作不同的处理，希望有一个框架，当需要对收到的数据作不同的解析时，只需要使用不同的解析器即可；同时希望在需要时，可以使用不同的解析器解析收到的数据。

经过改造，将例程中的TCP server改造，得到的框架如下：
 *
 *
 *
 */
#ifndef __SOCKET_MANAGER_H

#define __SOCKET_MANAGER_H

//#include <boost/signal.hpp>

//#include <boost/bind.hpp>

#include <string>

#include <algorithm>

#include <cstdlib>

#include <deque>

#include <iostream>

#include <list>

#include <set>

#include <boost/bind.hpp>

#include <boost/shared_ptr.hpp>

#include <boost/enable_shared_from_this.hpp>

#include <boost/thread.hpp>

#include <boost/thread/mutex.hpp>

#include "CCharBase.h"

#include <boost/asio.hpp>

using boost::asio::ip::tcp;

using namespace boost;

using namespace std;

//using namespace boost::interprocess;

//----------------------------------------------------------------------

class tcp_worker

{

protected:

	//字符解析器，字符解析器是每次单独生成的。但是frameparser在系统中只有一个，因此要求frameparser具有线程安全性

    shared_ptr<CCharObserverHolder> m_byte_parser;

	//	tcp::socket&			m_socket_;

public:

    tcp_worker (shared_ptr<CCharObserverHolder> byte_parser)

	{

		//		m_socket_ = socket;

        m_byte_parser = byte_parser;

	}

	virtual ~tcp_worker()

	{

	}

	virtual void write_buffer(unsigned char* buffer, int len) = 0;

};

//----------------------------------------------------------------------

class tcp_session_manager

{

private:

    typedef boost::shared_ptr<tcp_worker> 			tcp_worker_ptr;

	list<tcp_worker_ptr> participants_;

public:

    tcp_session_manager ()

	{

		participants_.clear();

	}

	void join(tcp_worker_ptr participant)

	{

		participants_.push_back(participant);

	}

	void leave(tcp_worker_ptr participant)

	{

		participants_.remove(participant);

	}

	list<tcp_worker_ptr>& Clients()

	{

		return participants_;

	}

	void WriteToAllClient(unsigned char* buffer, int length)

	{

         std::list<tcp_worker_ptr>::iterator it;

		for (it = participants_.begin(); it != participants_.end(); it++)

		{

			try

			{

				(*it)->write_buffer(buffer, length);

			} catch (boost::exception& ce)

			{

				cout << "write socket error" << diagnostic_information(ce)

						<< endl;

			}

		}

	}

};

//----------------------------------------------------------------------

class tcp_session: public tcp_worker,

        public boost::enable_shared_from_this<tcp_session >

{

	boost::recursive_mutex mutex;

	// scoped_lock;

public:

    tcp_session (

            boost::asio::io_service& io_service, tcp_session_manager& room, shared_ptr<CCharObserverHolder> parser) :

             tcp_worker (parser), socket_(io_service),sessions_(room)

	{

		//  write_buf = NULL;

	}

	tcp::socket& socket()

	{

		return socket_;

	}

	void start()

	{

        sessions_.join(this->shared_from_this());

		socket_.async_read_some(// socket_,

                boost::asio::buffer(read_msg_, max_msg_len),

                       boost::bind(

						&tcp_session::handle_read, this->shared_from_this(),

						boost::asio::placeholders::error,

						boost::asio::placeholders::bytes_transferred));

	}

	void write_buffer(unsigned char* buffer, int len)

	{

		//TODO 需要加同步，否则大数据量时会出问题，或是改成同步发送，添加同步发送的功能

		//    mutex.lock();

		try

		{

			//if (len < max_msg_len)

			{

				//memcpy(write_buf, buffer, len);

				socket_.write_some(//socket_,

						boost::asio::buffer(buffer, len));

				//boost::bind(&tcp_session::handle_write,this->shared_from_this(),boost::asio::placeholders::error));

			}

		} catch (boost::exception& ce)

		{

			cout << "write error" << boost::diagnostic_information(ce) << endl;

		}

	}

	void handle_read(const boost::system::error_code& error,

			size_t bytes_transferred)

	{

		if (!error)

		{

			std::cout << "bytes read:" << bytes_transferred << std::endl;

			std::cout << std::endl;

			//输出信息

			//			std::cout << data_ << std::endl;

			for (unsigned int i = 0; i < bytes_transferred; i++)

			{

				//			char buffer[10];

				//			_itoa(read_msg_[i],buffer,16);

				//			std::cout << buffer<<",";

                this->m_byte_parser->OnCharIn((void*) &socket_,

						read_msg_[i]);

			}

			std::cout << std::endl;

			socket_.async_read_some(//socket_,

					boost::asio::buffer(read_msg_, max_msg_len), boost::bind(

							&tcp_session::handle_read,

							this->shared_from_this(),

							boost::asio::placeholders::error,

							boost::asio::placeholders::bytes_transferred));

		} else

		{

			sessions_.leave(this->shared_from_this());

		}

	}

	void handle_write(const boost::system::error_code& error)

	{

		//    mutex.unlock();

		if (!error)

		{

			cout << "write ok";

		} else

		{

			cout << "socket error:" << error.message() << endl;

			sessions_.leave(this->shared_from_this());

		}

	}

private:

	tcp::socket socket_;

    tcp_session_manager& sessions_;

	enum

	{

		max_msg_len = 2048

	};

	char write_buf[max_msg_len];

	unsigned char read_msg_[max_msg_len];

};

//----------------------------------------------------------------------

class CSocketServer

{

public:

    typedef boost::shared_ptr<tcp_session >

			chat_session_ptr;

    CSocketServer (

			boost::asio::io_service& io_service, const tcp::endpoint& endpoint,

            shared_ptr<CCharObserverHolder> parser) :

		io_service_(io_service), acceptor_(io_service, endpoint), parser_(

				parser)

	{

		chat_session_ptr new_session(

                new tcp_session (io_service_,

						sessions_, parser_));

		acceptor_.async_accept(new_session->socket(), boost::bind(

				&CSocketServer::handle_accept, this, new_session,

				boost::asio::placeholders::error));

	}

	void handle_accept(chat_session_ptr session,

			const boost::system::error_code& error)

	{

		if (!error)

		{

			session->start();

            chat_session_ptr new_session(new tcp_session (io_service_, sessions_, parser_));

			acceptor_.async_accept(new_session->socket(), boost::bind(

					&CSocketServer::handle_accept, this, new_session,

					boost::asio::placeholders::error));

		} else

		{

			cout << "socket error:" << error.message() << endl;

		}

	}

    tcp_session_manager session_manager()

	{

		return sessions_;

	}

	;

	int client_count()

	{

		return sessions_.Clients().size();

	}

        ~CSocketServer()

        {

           acceptor_.close();

        }

private:

	boost::asio::io_service& io_service_;

	tcp::acceptor acceptor_;

    tcp_session_manager sessions_;

    shared_ptr<CCharObserverHolder> parser_;

};

//typedef template <typename CharParserT,typename FrameParserT> CSocketServer;

class CSocketManager

{

public:

    typedef CSocketServer SOCK_SVR_TYPE;

private:

	boost::asio::io_service io_service;

	//	FrameParserT*   m_frame ;

	//	using namespace std; // For atoi.

	shared_ptr<SOCK_SVR_TYPE> m_svr_socket;//(io_service, atoi(argv[1]),m_frame);

    shared_ptr<boost::thread> m_server_thread;

public:

    CSocketManager (string ip, int port,shared_ptr<CCharObserverHolder> m_parser)

	{

		try

		{

			//	boost::asio::io_service io_service;

			tcp::endpoint end_point(boost::asio::ip::address::from_string(ip),

					port);

			m_svr_socket = shared_ptr<CSocketManager::SOCK_SVR_TYPE> (

					new CSocketManager::SOCK_SVR_TYPE(io_service, end_point,

							m_parser));

			//	m_svr_socket->

            m_server_thread =  shared_ptr<boost::thread>(new boost::thread(boost::bind(&asio::io_service::run, &io_service)));

		} catch (boost::exception& ce)

		{

			cout << "socket error:" << diagnostic_information(ce) << endl;

		}

	}

        virtual ~CSocketManager()

        {

            //io_service_.post(boost::bind(&chat_client::do_close, this));

            io_service.stop();

            m_server_thread->join();

            m_server_thread.reset();

        }

	void SendToAllClient(unsigned char* buffer, int len)

	{

		m_svr_socket->session_manager().WriteToAllClient(buffer, len);

	}

};

#endif




#ifndef CCHARBASE_H
/*
 *在该框架中，只需要在实例化CSocketManager时，输入一个m_parser的实例即可。

而m_parser的实现，采用了Observer的模式，这样的话，只需要向m_parser中attach不同的观测器，即可实现不同功能的解析器
 * */
#define CCHARBASE_H

#include <list>

#include "boost/shared_ptr.hpp"

using namespace boost;

using namespace std;

class CCharParserBase

{

public:

    CCharParserBase()

    {

    }

    virtual ~CCharParserBase()

    {

    }

    virtual void OnCharIn(void* pID,unsigned char inChar) = 0;

};

class CCharObserverHolder

{

    list<shared_ptr<CCharParserBase> >   m_parsers;

public:

    CCharObserverHolder()

    {

    }

    virtual ~CCharObserverHolder()

    {

    }

    void attach(shared_ptr<CCharParserBase> parser)

    {

        m_parsers.push_back(parser);

    }

    void detach(shared_ptr<CCharParserBase> parser)

    {

        for(list<shared_ptr<CCharParserBase> >::iterator it = m_parsers.begin(); it != m_parsers.end(); it++)

        {

            if(*it == parser)

            {

                m_parsers.erase(it);

                break;

            }

        }

    }

    void OnCharIn(void* pID,unsigned char inChar)

    {

        //其实应该与detach同步，正常情况下不做detach，所以不处理

        for(list<shared_ptr<CCharParserBase> >::iterator it = m_parsers.begin(); it != m_parsers.end(); it++)

        {

            (*it)->OnCharIn(pID,inChar);

        }

    }

};

#endif // CCHARBASE_H


/*
 *
 * 实际应用时，首先申请一个CCharObserverHolder的实例，然后申请一个CCharParserBase的实例，attach到CCharObserverHolder中去，

示例如下：
*/
int main()
{
	CCharObserverHolder* m_parsers;
	m_parsers = shared_ptr<CCharObserverHolder>(new CCharObserverHolder());

	m_parsers->attach(shared_ptr<CCharParserBase1 >(new CCharParserBase1()));

	m_parsers->attach(shared_ptr<CCharParserBase2 >(new CCharParserBase2()));

	m_socket_server = new CSocketManager("0.0.0.0",port,m_parsers);
}
