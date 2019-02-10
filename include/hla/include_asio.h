#pragma once

#ifdef ASIO_STANDALONE
#include <asio.hpp>
namespace hla {
	using error_code = asio::error_code;
}
#else
#include <boost/asio.hpp>
namespace asio = boost::asio;
namespace hla {
	using error_code = boost::system::error_code;
}
#endif