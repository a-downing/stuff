cmake_minimum_required(VERSION 2.8.8)
project(enumthing)

set(CMAKE_CXX_STANDARD 20)
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

add_executable(enumthing main.cpp)

target_compile_options(enumthing PRIVATE
    -g
    -O2
    -Wall
    -Wextra
    -pedantic
)
