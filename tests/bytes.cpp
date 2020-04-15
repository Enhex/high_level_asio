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
			using type = std::array<unsigned char, 4>;
			type a{1,2,3,4};
			write_bytes(client_socket, a);

			type b;
			read_bytes(connection.socket, b);
			
			for (auto e : b) {
				std::cout << (int)e << ',';
			}

			assert(a == b);
		});

		auto fut1 = std::async([&context] { context.run(); });

		const auto endpoint = tcp::endpoint(asio::ip::address_v4({ 127,0,0,1 }), port);
		client_socket.connect(endpoint);

		fut1.wait();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}
}