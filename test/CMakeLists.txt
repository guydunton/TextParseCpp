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
	CombinedParserTest.cpp
)

set(HEADERS
	../include/Parser.hpp
	../include/ParserBuilder.hpp
	../include/Meta.hpp
	../include/ParserCore.hpp
	../include/SelectionContainer.hpp
	../include/CombinedParserCore.hpp
)

include_directories(../include)
add_executable(Tests ${SOURCES} ${HEADERS})

if (MSVC)
	target_link_libraries(Tests)
else()
	target_link_libraries(Tests --coverage)
endif()

add_test(Test Tests)
