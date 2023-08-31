cmake_minimum_required(VERSION 3.21)
project(clib C)
set(C_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON) # PIC

option(BUILD_SHARED_LIBS "Build libraries as SHARED" OFF)   # static by default
option(test "enable tests" OFF)

add_subdirectory(array)