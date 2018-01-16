#include <asio-utility/utility.h>

int main()
{
	asio::io_context context;
	
	// create a lot of work to be able to see if it uses multiple cores
	for (int i = 1000; i-- > 0;)
	{
		context.post([]{
			auto str = std::string("hello world! 123456789000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000\n");
			for (int n = 2500000; n-- > 0;)
				str.reserve();
		});
	}

	auto threads = thread_pool_run(context);
	join_threads(threads);
}