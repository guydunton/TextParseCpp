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

enable_testing()

add_subdirectory(test)
