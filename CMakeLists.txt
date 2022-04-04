cmake_minimum_required(VERSION 3.10)
project(compiler_new_programming_language_22_03)

set(CMAKE_CXX_STANDARD 20)
include_directories(include)

add_executable(compiler_new_programming_language_22_03
        src/main.cpp
        include/exceptions.h
        src/exceptions.cpp
        src/converter.cpp
        include/converter.h
        src/mixed.cpp
        include/mixed.h)
