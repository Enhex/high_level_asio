#include <hla/file.h>
#include <iostream>
#include <future>

using asio::ip::tcp;

constexpr unsigned short port = 27000;

void on_accept(tcp::socket& socket)
{
	auto content = read_file(socket);
	std::cout << std::string(content.begin(), content.end()) << '\n';
}

int main()
{
	asio::io_context context;

	tcp::acceptor acceptor(context, tcp::endpoint(tcp::v4(), port));

	tcp::socket socket(context);
	acceptor.async_accept(socket, [&](std::error_code ec) { on_accept(socket); });

	auto read_future = std::async([&context] { context.run(); });

	std::puts("writing...");
	{
		tcp::socket socket(context);
		const auto endpoint = tcp::endpoint(asio::ip::address_v4({127,0,0,1}), port);
		socket.connect(endpoint);

		{
			constexpr auto file_name = "test.txt";
			std::ofstream os(file_name, std::ios::binary);
			os << "hello world!";
		}

		write_file(socket, "test.txt");
	}
}