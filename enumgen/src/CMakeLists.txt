cmake_minimum_required(VERSION 2.8.8)
project(enumgentest)

set(CMAKE_CXX_STANDARD 17)
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

add_executable(test main.cpp)

target_compile_options(test PRIVATE
    -g
    -O2
    -Wall
    -Wextra
    -pedantic
)
