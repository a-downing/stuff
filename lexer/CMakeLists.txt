cmake_minimum_required(VERSION 2.8.8)
project(lexer)

set(CMAKE_CXX_STANDARD 20)

add_executable(lexer main.cpp)

target_compile_options(lexer PRIVATE
    -std=c++20
    -g
    -O2
    -Wall
    -Werror
    -Wextra
    -pedantic
)
