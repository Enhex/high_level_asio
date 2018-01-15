#include <asio-utility/string.h>
#include <iostream>
#include <future>

using asio::ip::tcp;

constexpr unsigned short port = 27000;

void on_accept(tcp::socket& socket)
{
	std::cout << read_string(socket) << '\n';
}

int main()
{
	asio::io_context context;

	tcp::acceptor acceptor(context, tcp::endpoint(tcp::v4(), port));

	tcp::socket socket(context);
	acceptor.async_accept(socket, [&](std::error_code ec) { on_accept(socket); });

	auto read_future = std::async([&context] { context.run(); });

	std::puts("writing string...");
	{
		tcp::socket socket(context);
		const auto endpoint = tcp::endpoint(asio::ip::address_v4({127,0,0,1}), port);
		socket.connect(endpoint);
		
		const std::string str = "hello world!";
		write_string(socket, str);
	}
}