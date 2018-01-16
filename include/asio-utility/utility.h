#pragma once

#include <asio.hpp>
#include <thread>
#include <vector>

constexpr auto localhost_v4_bytes = asio::ip::address_v4::bytes_type{ 127,0,0,1 };
const auto localhost_v4 = asio::ip::address_v4(localhost_v4_bytes);

// create N threads and call asio::io_context::run() in them
std::vector<std::thread> thread_pool_run(asio::io_context& context, unsigned threads_count)
{
	std::vector<std::thread> threads;

	for (; threads_count-- > 0;)
	{
		threads.emplace_back([&context] {
			context.run();
		});
	}

	return std::move(threads);
}

// create threads according to the number of available cores and call asio::io_context::run() in them
std::vector<std::thread> thread_pool_run(asio::io_context& context)
{
	return thread_pool_run(context, std::thread::hardware_concurrency());
}

void join_threads(std::vector<std::thread>& threads)
{
	for (auto& thread : threads)
		thread.join();
}