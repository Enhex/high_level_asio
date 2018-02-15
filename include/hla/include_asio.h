#pragma once

#ifdef ASIO_STANDALONE
#include <asio.hpp>
#else
#include <boost/asio.hpp>
namespace asio = boost::asio;
#endif