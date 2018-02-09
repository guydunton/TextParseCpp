cmake_minimum_required(VERSION 3.1)
project(tests)

if (MSVC)
	add_compile_options(/W4)
else()
	add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(CMAKE_CXX_STANDARD 11)

set(sources
	main.cpp
	parser_test.cpp
	text_test.cpp
	combined_parser_test.cpp
    invoke_test.cpp
    function_test.cpp
)

include_directories(../include)
add_executable(tests ${sources} ${include_headers})

if (MSVC)
	target_link_libraries(tests)
else()
	target_link_libraries(tests --coverage)
endif()

add_test(Test tests)
