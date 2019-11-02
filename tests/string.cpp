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

		asio::ip::tcp::socket client_socket(context);
		hla::connection connection(context); // server side connection

		acceptor.async_accept(connection.socket, [&](std::error_code ec)
		{

			{
				write_string(client_socket, "hello ");

				auto read_str = read_string(connection.socket);
				std::cout << read_str << '\n';
			}
			{
				const std::string str = "world!";
				write_null_string(client_socket, str);

				auto read_str = connection.read_null_string();
				std::cout << read_str << '\n';
			}
		});


		auto fut1 = std::async([&context] { context.run(); });

		std::puts("writing string...");

		const auto endpoint = tcp::endpoint(asio::ip::address_v4({ 127,0,0,1 }), port);
		client_socket.connect(endpoint);

		fut1.wait();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}
}