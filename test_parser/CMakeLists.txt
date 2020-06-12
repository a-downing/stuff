cmake_minimum_required(VERSION 2.8.8)
project(test_parser)

set(CMAKE_CXX_STANDARD 20)

add_executable(parser main.cpp)

target_include_directories(parser PRIVATE "../")

target_compile_options(parser PRIVATE
    -g
    -O0
    -Wall
    -Wextra
    -pedantic
)
