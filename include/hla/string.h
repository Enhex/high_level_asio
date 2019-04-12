#pragma once

/*TODO
- error overloads
- async overloads
- read_until overloads
*/

#include "hla/include_asio.h"
#include <string>

// write a std::string as a null-terminated string
template<typename SyncWriteStream>
void write_string(SyncWriteStream& stream, const std::string& str)
{
	asio::write(stream, asio::buffer(str.c_str(), str.size() + 1));
}


// read a std::string as a null-terminated string, with a re-usable asio::streambuf (make sure to clear it when needed)
//NOTE: data beyond the delimiter may be read into the buffer, so you need to use it for subsequent reads.
template<typename SyncReadStream>
std::string read_string(SyncReadStream& stream, asio::streambuf& sb)
{
	const auto str_size = asio::read_until(stream, sb, '\0') - 1;
	const auto bufs = sb.data();
	sb.consume(str_size + 1);
	return std::string(
		asio::buffers_begin(bufs),
		asio::buffers_begin(bufs) + str_size);
}

// simplified version without reusable asio::streambuf
template<typename SyncReadStream>
std::string read_string(SyncReadStream& stream)
{
	asio::streambuf sb;
	return read_string(stream, sb);
}