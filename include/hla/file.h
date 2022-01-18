#pragma once

/*TODO
- error overloads
- async overloads
- read_until overloads
*/

#include "hla/include_asio.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

namespace hla
{
	// write a file using size tag
	//TODO file size template parameter
	template<typename SyncWriteStream>
	void write_file(SyncWriteStream& stream, std::ifstream& file)
	{
		std::vector<char> const content(
			(std::istreambuf_iterator<char>(file)),
			std::istreambuf_iterator<char>());

		// use fixed 32 bit unsigned int
		auto const content_size = content.size();
		if (content_size > std::numeric_limits<uint32_t>::max()) {
			throw std::length_error("File size too big to be represented by uint32_t.");
		}
		auto const file_size = uint32_t(content_size);

		hla::error_code error;

		// write file size
		asio::write(stream, asio::buffer(&file_size, sizeof(file_size)), error);
		if (error)
			std::cerr << error.message();

		// write file
		asio::write(stream, asio::buffer(content, file_size), error);
		if (error)
			std::cerr << error.message();
	}

	template<typename SyncWriteStream>
	void write_file(SyncWriteStream& stream, std::filesystem::path const& path)
	{
		std::ifstream file(path, std::ios::binary);
		write_file(stream, file);
	}

	// read a file using size tag
	template<typename SyncReadStream>
	std::vector<std::byte> read_file(SyncReadStream& stream)
	{
		hla::error_code error;

		// read file size
		uint32_t file_size = 0;
		asio::read(stream, asio::buffer(&file_size, sizeof(file_size)), error);
		if (error)
			std::cerr << error.message();

		// read file
		std::vector<std::byte> content(file_size);
		asio::read(stream, asio::buffer(content, file_size), error);
		if (error)
			std::cerr << error.message();

		return content;
	}
}