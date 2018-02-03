cmake_minimum_required(VERSION 3.1)
project(Tests)

if (MSVC)
	add_compile_options(/W4)
else()
	add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(CMAKE_CXX_STANDARD 11)

set(SOURCES
	main.cpp
	ParserTest.cpp
	TextTest.cpp
)

set(HEADERS
	../include/ParserBuilder.hpp
	../include/Meta.hpp
	../include/Matcher.hpp
	../include/SelectionContainer.hpp
)

include_directories(../include)
add_executable(Tests ${SOURCES} ${HEADERS})

if (MSVC)
	target_link_libraries(Tests)
else()
	target_link_libraries(Tests --coverage)
endif()

add_test(Test Tests)
