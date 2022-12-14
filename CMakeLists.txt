cmake_minimum_required(VERSION 3.16)

project(clib C)

if(NOT WIN32)
set(target "linux" CACHE STRING "Choose one of 'linux'(default),'windows', 'macos'")
endif()

option(example "build examples" OFF)
option(test "build tests" OFF)
option(zig "build using zig compiler" OFF)

set(CMAKE_C_STANDARD_REQUIRED c17)
if(NOT WIN32 AND zig)
    set(CMAKE_C_COMPILER ${CMAKE_SOURCE_DIR}/bin/zig-cc)
    message(${target})
    if(${target} STREQUAL "windows")
        set(CMAKE_C_COMPILER_ARG1 --target=x86_64-windows-gnu)
    elseif(${target} STREQUAL "macos")
        set(CMAKE_C_COMPILER_ARG1 --target=x86_64-macos-gnu)
    elseif(${target} STREQUAL "linux")
        set(CMAKE_C_COMPILER_ARG1 --target=x86_64-linux-gnu)
    else()
        message(FATAL_ERROR "target: Choose one of 'linux'(default),'windows', 'macos'")
    endif()
    set(CMAKE_AR ${CMAKE_SOURCE_DIR}/bin/zig-ar)
    set(CMAKE_RANLIB ${CMAKE_SOURCE_DIR}/bin/zig-ranlib)
endif()

add_subdirectory(lib)

if(example)
    add_subdirectory(example)
endif()

if(test)
    add_subdirectory(test)
endif()