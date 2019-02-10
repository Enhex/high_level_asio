#pragma once

/*TODO
- async overloads
- read_until overloads
*/

#include "hla/include_asio.h"
#include <string>

// write a given object's memory layout bytes
template<typename SyncWriteStream, typename T>
void write_bytes(SyncWriteStream& stream, const T& value)
{
	asio::write(stream, asio::buffer(&value, sizeof(value)));
}


// read a given object's memory layout bytes
template<typename SyncReadStream, typename T>
void read_bytes(SyncReadStream& stream, T& value)
{
	asio::read(stream, asio::buffer(&value, sizeof(value)));
}

template<typename T, typename SyncReadStream>
T read_bytes(SyncReadStream& stream)
{
	T value;
	read_bytes(stream, value);
	return value;
}