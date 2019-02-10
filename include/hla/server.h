#pragma once

#include "hla/include_asio.h"

namespace hla
{
	// accept connections on a given port with a callback
	struct server
	{
		using tcp = asio::ip::tcp;

		server(asio::io_context& context, uint16_t port) :
			acceptor(context, tcp::endpoint(asio::ip::tcp::v4(), port)),
			socket(context)
		{
		}

		asio::ip::tcp::acceptor acceptor;
		tcp::socket socket;

		// accept one new connection at a time asynchronously
		//NOTE: move the socket in OnAccept to control its life-time
		template<typename OnAccept>
		void accept(OnAccept&& on_accept)
		{
			socket = tcp::socket(acceptor.get_io_context()); // create a new socket in case the last one was moved
			acceptor.async_accept(socket, [&, on_accept = std::move(on_accept)](hla::error_code /*ec*/) {
				on_accept(socket);
				accept(std::move(on_accept));
			});
		}
	};
}