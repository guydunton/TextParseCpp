cmake_minimum_required(VERSION 3.1)
project(TextMatch)

if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

if (NOT MSVC)
        option(ENABLE_COVERAGE "Enable coverage reporting for gcc/clang" FALSE
)
        if (ENABLE_COVERAGE)
                add_compile_options(--coverage -O0)
        endif()
endif()

file(GLOB_RECURSE include_headers "include/*.hpp")
source_group("include_headers" FILES ${include_headers})

enable_testing()

include_directories(include)

add_subdirectory(test)
add_subdirectory(examples)