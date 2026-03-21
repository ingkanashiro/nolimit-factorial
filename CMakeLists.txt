cmake_minimum_required(VERSION 4.1)
project(factorial)

set(CMAKE_CXX_STANDARD 20)

set(BOOST_ROOT "C:/msys64/mingw64")
set(Boost_INCLUDE_DIR "C:/msys64/mingw64/include")
set(Boost_LIBRARY_DIR "C:/msys64/mingw64/lib")

find_package(Boost REQUIRED)

add_executable(factorial main.cpp)
target_link_libraries(factorial PRIVATE Boost::boost)
