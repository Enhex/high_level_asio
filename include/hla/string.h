#pragma once

/*TODO
- error overloads
- async overloads
- read_until overloads
*/

#include "hla/include_asio.h"
#include "object.h"
#include <string>

// write null-terminated string.
//NOTE: when reading with a asio::streambuf, note that the streambuf can read beyond the delimiter, so make sure it's being used for subsequent reads!
template<typename SyncWriteStream>
void write_null_string(SyncWriteStream& stream, std::string const& str)
{
	asio::write(stream, asio::buffer(str.c_str(), str.size() + 1));
}

template<typename SyncWriteStream, typename Size = uint32_t>
void write_string(SyncWriteStream& stream, const std::string& str)
{
	Size const size = str.size();
	write_bytes(stream, size);
	asio::write(stream, asio::buffer(str.data(), size));
}

template<typename SyncReadStream, typename Size = uint32_t>
std::string read_string(SyncReadStream& stream)
{
	std::string str;

	auto size = read_bytes<Size>(stream);
	str.resize(size);
	asio::read(stream, asio::buffer(str.data(), str.size()));

	return str;
}