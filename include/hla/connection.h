#pragma once

#include "hla/include_asio.h"
#include <vector>
#include <hla/string.h>

namespace hla
{
	struct connection
	{
		connection(asio::io_context& context) :
			socket(context)
		{
		}

		connection(asio::ip::tcp::socket&& socket) :
			socket(std::move(socket))
		{
		}

		asio::ip::tcp::socket socket;

		// reusable buffer
		asio::streambuf read_buffer;

		void connect(const char* address, uint16_t port)
		{
			socket.connect(asio::ip::tcp::endpoint(asio::ip::make_address(address), port));
		}

		// directly reads the type's bytes
		//TODO use enable_if
		template<typename T>
		void read(T& value)
		{
			constexpr auto t_size = sizeof(T);
			read_buffer.commit(t_size);

			auto buffers = read_buffer.data();
			auto buffers_iter = asio::buffers_begin(buffers);

			for (auto i = 0; i < t_size; ++i) {
				reinterpret_cast<std::array<std::byte, t_size>&>(value)[i] = static_cast<std::byte>(*buffers_iter);
				++buffers_iter;
			}

			read_buffer.consume(t_size);
		}

		template<typename T>
		T read()
		{
			T value;
			read(value);
			return value;
		}

		// read null-teriminated string
		std::string read_null_string()
		{
			asio::streambuf read_buffer;
			const auto str_size = asio::read_until(socket, read_buffer, '\0') - 1;
			const auto bufs = read_buffer.data();
			read_buffer.consume(str_size + 1);
			return std::string(
				asio::buffers_begin(bufs),
				asio::buffers_begin(bufs) + str_size);
		}
	};
}