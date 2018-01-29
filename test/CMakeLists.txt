cmake_minimum_required(VERSION 3.1)
project(Tests)

if (MSVC)
	add_compile_options(/W4)
else()
	add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(CMAKE_CXX_STANDARD 11)


include_directories(../include)
add_executable(Tests main.cpp MatcherTest.cpp TextTest.cpp)
target_link_libraries(Tests --coverage)
add_test(Test Tests)
