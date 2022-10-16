cmake_minimum_required(VERSION 3.16)

project(clib C)
set(CMAKE_C_STANDARD_REQUIRED c17)

option(example "build examples" OFF)
option(test "build tests" OFF)

add_subdirectory(lib)

if(example)
    add_subdirectory(example)
endif()

if(test)
    add_subdirectory(test)
endif()