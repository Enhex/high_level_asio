set source=%~dp0

cd ..
mkdir high_level_asio-tests-build
cd high_level_asio-tests-build

conan install ../high_level_asio/tests -s build_type=Debug -s arch=x86_64

cd %source%/tests
premake5 vs2019 --location="../../high_level_asio-tests-build/"