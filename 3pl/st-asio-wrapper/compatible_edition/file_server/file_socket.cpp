
//configuration
#define ST_ASIO_SERVER_PORT		5051
#define ST_ASIO_AUTO_CLEAR_CLOSED_SOCKET //auto clear closed clients
#define ST_ASIO_ENHANCED_STABILITY
#define ST_ASIO_CLOSED_SOCKET_MAX_DURATION	0
#define ST_ASIO_WANT_MSG_SEND_NOTIFY
#define ST_ASIO_DEFAULT_PACKER	replaceable_packer
//configuration

#include "file_socket.h"

file_socket::file_socket(i_server& server_) : st_server_socket(server_) {}
file_socket::~file_socket() {trans_end();}

void file_socket::reset() {trans_end(); st_server_socket::reset();}

//msg handling
#ifndef ST_ASIO_FORCE_TO_USE_MSG_RECV_BUFFER
//we can handle msg very fast, so we don't use recv buffer
bool file_socket::on_msg(out_msg_type& msg) {handle_msg(msg); return true;}
#endif
bool file_socket::on_msg_handle(out_msg_type& msg, bool link_down) {handle_msg(msg); return true;}
//msg handling end

#ifdef ST_ASIO_WANT_MSG_SEND_NOTIFY
void file_socket::on_msg_send(in_msg_type& msg)
{
	BOOST_AUTO(buffer, boost::dynamic_pointer_cast<file_buffer>(msg.raw_buffer()));
	if (NULL != buffer)
	{
		buffer->read();
		if (buffer->empty())
			trans_end();
		else
			direct_send_msg(msg, true);
	}
}
#endif

void file_socket::trans_end()
{
	state = TRANS_IDLE;
	if (NULL != file)
	{
		fclose(file);
		file = NULL;
	}
}

void file_socket::handle_msg(out_msg_ctype& msg)
{
	if (msg.size() <= ORDER_LEN)
	{
		printf("wrong order length: " ST_ASIO_SF ".\n", msg.size());
		return;
	}

	switch (*msg.data())
	{
	case 0:
		if (TRANS_IDLE == state)
		{
			trans_end();

			char buffer[ORDER_LEN + DATA_LEN];
			*buffer = 0; //head

			file = fopen(boost::next(msg.data(), ORDER_LEN), "rb");
			if (NULL != file)
			{
				fseeko(file, 0, SEEK_END);
				fl_type length = ftello(file);
				memcpy(boost::next(buffer, ORDER_LEN), &length, DATA_LEN);
				state = TRANS_PREPARE;
			}
			else
			{
				memset(boost::next(buffer, ORDER_LEN), -1, DATA_LEN);
				printf("can't not open file %s!\n", boost::next(msg.data(), ORDER_LEN));
			}

			send_msg(buffer, sizeof(buffer), true);
		}
		break;
	case 1:
		if (TRANS_PREPARE == state && NULL != file && ORDER_LEN + OFFSET_LEN + DATA_LEN == msg.size())
		{
			fl_type offset;
			memcpy(&offset, boost::next(msg.data(), ORDER_LEN), OFFSET_LEN);
			fl_type length;
			memcpy(&length, boost::next(msg.data(), ORDER_LEN + OFFSET_LEN), DATA_LEN);
			if (offset >= 0 && length > 0 && offset + length <= ftello(file))
			{
				state = TRANS_BUSY;
				fseeko(file, offset, SEEK_SET);
				direct_send_msg(replaceable_buffer(boost::make_shared<file_buffer>(file, length)), true);
			}
		}
		break;
	case 2:
		printf("client says: %s\n", boost::next(msg.data(), ORDER_LEN));
		break;
	default:
		break;
	}
}

//restore configuration
#undef ST_ASIO_SERVER_PORT
#undef ST_ASIO_AUTO_CLEAR_CLOSED_SOCKET
#undef ST_ASIO_ENHANCED_STABILITY
#undef ST_ASIO_CLOSED_SOCKET_MAX_DURATION
#undef ST_ASIO_WANT_MSG_SEND_NOTIFY
#undef ST_ASIO_DEFAULT_PACKER
//restore configuration
