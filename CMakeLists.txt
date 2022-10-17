cmake_minimum_required(VERSION 3.16)

project(clib C)

option(windows "build for Windows OS target" OFF)
option(macos "build for Mac OS target" OFF)
option(linux "build for linux OS target" ON)

set(CMAKE_C_STANDARD_REQUIRED c17)
set(CMAKE_C_COMPILER ${CMAKE_SOURCE_DIR}/bin/zig-cc)
if(windows)
    set(CMAKE_C_COMPILER_ARG1 --target=x86_64-windows-gnu)
elseif(macos)
    set(CMAKE_C_COMPILER_ARG1 --target=x86_64-macos-gnu)
elseif(linux)
    set(CMAKE_C_COMPILER_ARG1 --target=x86_64-linux-gnu)
endif()
set(CMAKE_AR ${CMAKE_SOURCE_DIR}/bin/zig-ar)
set(CMAKE_RANLIB ${CMAKE_SOURCE_DIR}/bin/zig-ranlib)

option(example "build examples" OFF)
option(test "build tests" OFF)

add_subdirectory(lib)

if(example)
    add_subdirectory(example)
endif()

if(test)
    add_subdirectory(test)
endif()