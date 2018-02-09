cmake_minimum_required(VERSION 3.1)
project(examples)

set(CMAKE_CXX_STANDARD 11)

add_executable(simple_parser simple_parser.cpp ${include_headers})
