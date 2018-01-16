#pragma once

/*TODO
- error overloads
- async overloads
- read_until overloads
*/

#include <asio.hpp>
#include <string>

// write a std::string as a null-terminated string
template<typename SyncWriteStream>
void write_string(SyncWriteStream& stream, const std::string& str)
{
	asio::write(stream, asio::buffer(str.c_str(), str.size() + 1));
}


// read a std::string as a null-terminated string, with a re-usable asio::streambuf (make sure to clear it when needed)
template<typename SyncReadStream>
std::string read_string(SyncReadStream& stream, asio::streambuf& sb)
{
	const auto str_size = asio::read_until(stream, sb, '\0') - 1;
	const auto bufs = sb.data();
	return std::string(
		asio::buffers_begin(bufs),
		asio::buffers_begin(bufs) + str_size);
}

// read a std::string as a null-terminated string
template<typename SyncReadStream>
std::string read_string(SyncReadStream& stream)
{
	asio::streambuf sb;
	return read_string(stream, sb);
}
