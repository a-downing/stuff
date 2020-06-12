cmake_minimum_required(VERSION 2.8.8)
project(pratt_parser)

set(CMAKE_CXX_STANDARD 20)

add_executable(parser main.cpp)

target_compile_options(parser PRIVATE
    -g
    -O2
    -Wall
    -Wextra
    -pedantic
)
