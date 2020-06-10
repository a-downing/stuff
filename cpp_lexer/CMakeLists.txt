cmake_minimum_required(VERSION 2.8.8)
project(cpp_lexer)

set(CMAKE_CXX_STANDARD 20)

add_executable(cpp_lexer main.cpp)

target_include_directories(cpp_lexer PRIVATE "../lexer")

target_compile_options(cpp_lexer PRIVATE
    -g
    -O2
    -Wall
    -Wextra
    -pedantic
)
