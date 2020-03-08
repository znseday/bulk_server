#pragma once

#include <memory>
#include <algorithm>

#include <boost/asio.hpp>

#include "async.h"

constexpr auto MAX_DATA_SIZE = 1024;

namespace ba = boost::asio;

class RequestHandler : public std::enable_shared_from_this<RequestHandler> 
{
private:
	ba::ip::tcp::socket Socket;
	std::array<char, MAX_DATA_SIZE> Data = {0};
	async::handle_t Handle;

	void do_read();

public:
	explicit RequestHandler(ba::ip::tcp::socket _socket, std::size_t _BulkSize)
		: Socket(std::move(_socket)), Handle(async::connect(_BulkSize))
	{
		// memcpy(&Data, 0, sizeof(Data));
	}

	void Start();

};

class BulkServer 
{
private:

	void do_accept();

	boost::asio::ip::tcp::acceptor Acceptor;
	ba::ip::tcp::socket Socket;
	std::size_t BulkSize = 0;

public:
	BulkServer(ba::io_service& _io_service, const ba::ip::tcp::endpoint& _endpoint,
		std::size_t _BulkSize)
		: Acceptor(_io_service, _endpoint), Socket(_io_service), BulkSize(_BulkSize)
	{
		do_accept();
	}


};