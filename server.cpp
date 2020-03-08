#include "server.h"

#include <iostream>

using namespace std;
using boost::asio::ip::tcp;

void RequestHandler::Start()
{
	do_read();
}

void RequestHandler::do_read() {
	//auto self(shared_from_this());
	auto self(shared_from_this());
	Socket.async_read_some(
		ba::buffer(Data),
		[this, self](boost::system::error_code ec, std::size_t length)
		{
			if (!ec)
			{
				async::receive(Handle, Data.data(), length);
				do_read();
			}
			else 
			{
				async::disconnect(Handle);
			}
		}
	);
}


void BulkServer::do_accept()
{
	Acceptor.async_accept(Socket,
		[this](boost::system::error_code ec)
		{
			if (!ec)
			{
				std::make_shared<RequestHandler>(std::move(Socket), BulkSize)->Start();
			}

			do_accept();
		}
	);
}