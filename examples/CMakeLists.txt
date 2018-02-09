cmake_minimum_required(VERSION 3.1)
project(examples)

set(CMAKE_CXX_STANDARD 11)

add_executable(simple_parser simple_parser.cpp ${include_headers})

if (MSVC)
	target_link_libraries(simple_parser)
else()
	target_link_libraries(simple_parser --coverage)
endif()
