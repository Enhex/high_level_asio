#include <future>
#include <hla/connection.h>
#include <hla/string.h>
#include <iostream>

using asio::ip::tcp;

constexpr unsigned short port = 27000;

int main()
{
	try
	{
		asio::io_context context;

		tcp::acceptor acceptor(context, tcp::endpoint(tcp::v4(), port));

		tcp::socket socket(context);
		tcp::socket connect_socket(context);

		acceptor.async_accept(socket, [&](std::error_code ec)
		{
			hla::connection socket_con(std::move(socket));

			const std::string str = "hello ";
			write_string(connect_socket, str);

			auto str1 = socket_con.read_string();
			std::cout << str1 << '\n';

			write_string(connect_socket, "world!");

			auto str2 = socket_con.read_string();
			std::cout << str2 << '\n';
		});


		auto fut1 = std::async([&context] { context.run(); });

		std::puts("writing string...");

		const auto endpoint = tcp::endpoint(asio::ip::address_v4({ 127,0,0,1 }), port);
		connect_socket.connect(endpoint);
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}
}