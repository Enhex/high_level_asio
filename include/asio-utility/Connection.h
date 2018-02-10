#pragma once

#include <asio.hpp>
#include <vector>

struct Connection
{
	Connection(asio::io_context& context) :
		socket(context)
	{
	}

	Connection(asio::ip::tcp::socket&& socket) :
		socket(std::move(socket))
	{
	}

	asio::ip::tcp::socket socket;

	// reusable buffer
	std::vector<std::byte> read_buffer;


	// prepare the fixed buffer for reading
	std::vector<std::byte>& prepare_buffer(size_t size)
	{
		read_buffer.resize(size);
		return read_buffer;
	}
};
