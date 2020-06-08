cmake_minimum_required(VERSION 2.8.8)
project(parser)

set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-variable -Wno-unused-parameter")

add_executable(parser main.cpp)

target_compile_options(parser PRIVATE
    -g
    -O2
    -Wall
    -Werror
    -Wextra
    -pedantic
)
