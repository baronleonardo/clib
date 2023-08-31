cmake_minimum_required(VERSION 3.21)
project(clib C)
set(CMAKE_C_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON) # PIC
if (MSVC)
    # warning level 4
    add_compile_options(/W4)
else()
    # additional warnings
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

option(BUILD_SHARED_LIBS "Build libraries as SHARED" OFF)   # static by default
option(test "enable tests" OFF)

add_subdirectory(array)