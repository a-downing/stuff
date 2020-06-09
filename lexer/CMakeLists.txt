cmake_minimum_required(VERSION 2.8.8)
project(lexer)

set(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-variable -Wno-unused-parameter")

add_executable(lexer main.cpp)

target_compile_options(lexer PRIVATE
    -g
    -O2
    -Wall
    -Werror
    -Wextra
    -pedantic
)
