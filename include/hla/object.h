#pragma once

/*TODO
- async overloads
- read_until overloads
*/

#include "hla/include_asio.h"

// write a given object's memory layout bytes
template<typename SyncWriteStream, typename T>
void write_bytes(SyncWriteStream& stream, const T& value)
{
	asio::write(stream, asio::buffer(&value, sizeof(value)));
}

template<typename SyncWriteStream, typename T>
void write_bytes(SyncWriteStream& stream, const T& value, hla::error_code& ec)
{
	asio::write(stream, asio::buffer(&value, sizeof(value)), ec);
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

template<typename T>
auto get_buffer(T& value)
{
	return asio::buffer(&value, sizeof(value));
}

//NOTE: make sure value outlives callback
template<typename T, typename SyncReadStream>
void async_read_bytes(SyncReadStream& stream, T& buffer, std::function<void()> callback)
{
	asio::async_read(stream, buffer,
		[callback](hla::error_code const& ec, std::size_t bytes_transferred)
	{
		callback();
	});
}


// write variable length sequence of bytes
template<typename SyncWriteStream, typename T, typename Size = typename T::size_type>
void write_sequence(SyncWriteStream& stream, T const& seq)
{
	write_bytes(stream, static_cast<Size>(seq.size()));
	asio::write(stream, asio::buffer(seq.data(), sizeof(typename T::value_type) * seq.size()));
}

//NOTE: sequence type T must have resize() data() and size() member functions
template<typename T, typename Size = typename T::size_type, typename SyncReadStream>
T read_sequence(SyncReadStream& stream)
{
	T seq;

	auto size = read_bytes<Size>(stream);
	seq.resize(size);
	asio::read(stream, asio::buffer(seq.data(), sizeof(typename T::value_type) * seq.size()));

	return seq;
}
