cd ..
mkdir asio-utility-tests-build
cd asio-utility-tests-build

conan install ../asio-utility/tests
cmake ../asio-utility/tests -G "Visual Studio 15 Win64"