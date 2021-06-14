#include <hla/server.h>
#include <iostream>
#include <future>

using asio::ip::tcp;

constexpr unsigned short port = 27000;
constexpr int num_connections = 5;

int main()
{
	std::puts("IPv4");
	{
		int connections = num_connections;

		asio::io_context context;
		auto server = hla::server(context, tcp::endpoint(asio::ip::tcp::v4(), port));

		server.accept([&](tcp::socket& socket)
		{
			std::cout << "accepted connection from: " << socket.local_endpoint().address().to_string() << '\n';
			if (--connections == 0)
				context.stop();
		});

		auto read_future = std::async([&context] { context.run(); });

		for(int n = num_connections; n-- > 0;)
		{
			std::puts("connecting to server...");
			tcp::socket socket(context);
			const auto endpoint = tcp::endpoint(asio::ip::address_v4({127,0,0,1}), port);
			socket.connect(endpoint);
		}
	}

	std::puts("IPv6");
	{
		int connections = num_connections;

		asio::io_context context;
		auto server = hla::server(context, port);

		server.accept([&](tcp::socket& socket)
		{
			std::cout << "accepted connection from: " << socket.local_endpoint().address().to_string() << '\n';
			if (--connections == 0)
				context.stop();
		});

		auto read_future = std::async([&context] { context.run(); });

		for(int n = num_connections; n-- > 0;)
		{
			std::puts("connecting to server...");
			tcp::socket socket(context);
			const auto endpoint = tcp::endpoint(asio::ip::make_address_v6("::1"), port);
			socket.connect(endpoint);
		}
	}
}