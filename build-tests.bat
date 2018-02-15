cd ..
mkdir high_level_asio-tests-build
cd high_level_asio-tests-build

conan install ../high_level_asio/tests
cmake ../high_level_asio/tests -G "Visual Studio 15 Win64"